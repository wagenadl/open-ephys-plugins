/* artifilt/LocalFit.C: part of meabench, an MEA recording and analysis tool
** Copyright (C) 2000-2003  Daniel Wagenaar (wagenaar@caltech.edu)
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

// LocalFit.C

#include "LocalFit.h"
#include <iostream>

#define ASYM_NOT_CHI2 1
#ifndef THIRDORDER
#define THIRDORDER 1
#endif

#include <math.h>

//--------------------------------------------------------------------
// inline functions
//
inline void LocalFit::update_X012() {
  real_t y_new = source[t_stream+tau];
  real_t y_old = source[t_stream-tau-1];
  X0 += y_new - y_old;
  X1 += tau_plus_1*y_new - minus_tau*y_old - X0;
  X2 += tau_plus_1_squared*y_new - minus_tau_squared*y_old - X0 - 2*X1;
}

inline void LocalFit::calc_alpha0() {
  alpha0 = real_t(T4*X0 - T2*X2)/real_t(T0*T4-T2*T2);
}

//--------------------------------------------------------------------
// Other LocalFit methods
//
LocalFit::LocalFit(CyclBuf<raw_t> const &source0,
                   CyclBuf<raw_t> &dest0,
                   timeref_t t_start,
		   raw_t threshold0, timeref_t tau0,
		   timeref_t t_blankdepeg0, timeref_t t_ahead0,
		   timeref_t t_chi20):
  source(source0), dest(dest0), y_threshold(threshold0), tau(tau0),
  t_blankdepeg(t_blankdepeg0), t_ahead(t_ahead0), t_chi2(t_chi20) {
  state = PEGGED;
  usenegv = false;
  t_peg = t_stream = t_start;
  init_T();
  setthreshold(threshold0);
  setrail(RAIL1, RAIL2);
}

void LocalFit::setusenegv(bool t) {
  usenegv = t;
}

void LocalFit::reset(timeref_t t_start) {
  t_peg = t_stream = t_start;
  state=PEGGED;
}

void LocalFit::setrail(raw_t r1, raw_t r2) {
  rail1 = r1;
  rail2 = r2;
}  
void LocalFit::setthreshold(raw_t thr) {
  y_threshold = thr;
#if ASYM_NOT_CHI2
  my_thresh = 3.92 * t_chi2 * y_threshold*y_threshold; // 95% conf limit
#else
  my_thresh = (t_chi2-4) * y_threshold*y_threshold;
#endif
}  

void LocalFit::init_T() {
  tau_plus_1 = tau+1;
  tau_plus_1_squared = tau_plus_1 * tau_plus_1;
  tau_plus_1_cubed = tau_plus_1_squared * tau_plus_1;
  minus_tau = -tau;
  minus_tau_squared = minus_tau * minus_tau;
  minus_tau_cubed = minus_tau_squared * minus_tau;
    
  T0=T2=T4=T6=0;
  for (int_t t=-tau; t<=tau; t++) {
    int_t t2=t*t;
    int_t t4=t2*t2;
    int_t t6=t4*t2;
    T0+=1;
    T2+=t2;
    T4+=t4;
    T6+=t6;
  }
}

timeref_t LocalFit::process(timeref_t t_limit) {
  state=statemachine(t_limit, state);
  return t_stream;
}

timeref_t LocalFit::forcepeg(timeref_t t_from, timeref_t t_to) {
  state=statemachine(t_from-tau, state);
  if (state==OK) {
    // goto state PEGGING
      t0=t_stream-1;
      calc_X3();
      calc_alpha0123();
      statemachine(t_from, PEGGING);
  }
  t0=t_to;
  state=statemachine(t_to, FORCEPEG);
  return t_stream;
}

LocalFit::State LocalFit::statemachine(timeref_t t_limit, State s) {

  /* This is a straightforward implementation of the statemachine I
     described on 9/9/01.
   * //// mark boundaries program flow does not pass through.
   * On exit, t_stream == t_limit.
  */
  //  timeref_t t_check=0; //DBG
  switch (s) {
  case OK: goto l_OK;
  case PEGGED: goto l_PEGGED;
  case PEGGING: goto l_PEGGING;
  case TOOPOOR: goto l_TOOPOOR;
  case DEPEGGING: goto l_DEPEGGING;
  case FORCEPEG: goto l_FORCEPEG;
  case BLANKDEPEG: goto l_BLANKDEPEG;
  default: throw Error("LocalFit","BUG! Bad State");
  }

//////////////////////////////////////////////////
 l_PEGGED: {
    if (t_stream>=t_limit)
      return PEGGED;
    if (ispegged(source[t_stream])) {
      dest[t_stream] = 0;
      t_stream++;
      goto l_PEGGED;
    }
    for (int dt=1; dt<=2*tau; dt++)
      if (ispegged(source[t_stream+dt])) {
	t0 = t_stream+dt;
	goto l_FORCEPEG;
      }
    t0 = t_stream + tau;
    calc_X012(); calc_X3();
    calc_alpha0123();
    toopoorcnt=TOOPOORCNT;
    goto l_TOOPOOR;
  }

  throw Error("LocalFit","Code breach");
  
//////////////////////////////////////////////////
 l_TOOPOOR: {
    if (t_stream>=t_limit) 
      return TOOPOOR;
#if ASYM_NOT_CHI2
    real_t asym=0;
    real_t sig=0;
    for (int_t i=0; i<t_chi2; i++) {
      timeref_t t_i = t_stream + i;
      int_t dt = t_i - t0;
      int_t dt2 = dt*dt;
      int_t dt3 = dt*dt2;
      real_t dy = alpha0 + alpha1*dt + alpha2*dt2 + alpha3*dt3 - source[t_i];
      asym+=dy;
      sig+=dy*dy;
    }
    //    fprintf(stderr,"TOOPOOR: t=%.2f t0=%.2f asym/sqrt(t)=%g [t_chi2=%i sqrt(sig/t)=%g]\n",
    //	    t_stream/25.,t0/25.,asym/sqrt(t_chi2+0.),t_chi2,sqrt(sig/t_chi2));
    //    fprintf(stderr,"    alpha = %g  %g  %g  %g\n",alpha0,alpha1,alpha2,alpha3);
    asym *= asym;
    if (asym<my_thresh)
      toopoorcnt--;
    else
      toopoorcnt = TOOPOORCNT;
    if (toopoorcnt<=0 && asym < my_thresh/3.92) {
      if (usenegv) {
        int dt = t_stream - t0;
        int dt2 = dt*dt;
        int dt3 = dt*dt2;
        negv = source[t_stream]
          < raw_t(alpha0 + alpha1*dt + alpha2*dt2 + alpha3*dt3);
      }
      calc_X012(); calc_X3(); // for numerical stability problem!
      goto l_BLANKDEPEG;
    }
#else // ASYM_NOT_CHI2 is false
    real_t chi2=0;
    for (int_t i=0; i<t_chi2; i++) {
      timeref_t t_i = t_stream+t_blankdepeg+i;
      int_t dt = t_i - t0;
      int_t dt2 = dt*dt;
      int_t dt3 = dt*dt2;
      real_t dy = alpha0 + alpha1*dt + alpha2*dt2 + alpha3*dt3 - source[t_i];
      chi2 + =dy*dy;
    }
    ////    fprintf(stderr,"TOOPOOR: chi2=%g [%2f-%2f]\n",chi2,
    ////	    (t_stream+t_blankdepeg)/25.,(t_stream+t_blankdepeg+TOOPOORCNT)/25.);
    if (chi2 < my_thresh) {
      if (usenegv) {
        int dt = t_stream - t0;
        int dt2 = dt*dt;
        int dt3 = dt*dt2;
        negv = source[t_stream]
          < raw_t(alpha0 + alpha1*dt + alpha2*dt2 + alpha3*dt3);
      }
      goto l_BLANKDEPEG;
    }
#endif // ASYM_NOT_CHI2

//    int dt=t_stream-t0; int dt2=dt*dt; int dt3=dt*dt2;
//    dest[t_stream]=source[t_stream]-int(alpha0 + alpha1*dt + alpha2*dt2 + alpha3*dt3);
    dest[t_stream] = 0;
    t_stream++; t0++;
    if (ispegged(source[t0+tau])) {
      t0 = t0 + tau;
      goto l_FORCEPEG;
    }
    update_X0123();
    calc_X012(); calc_X3(); // for numerical stability problem!
    calc_alpha0123();
    goto l_TOOPOOR;
  }

  throw Error("LocalFit","Code breach");

//////////////////////////////////////////////////
 l_FORCEPEG: {
    if (t_stream>=t_limit)
      return FORCEPEG;
    if (t_stream>=t0)
      goto l_PEGGED;
    dest[t_stream] = 0;
    t_stream++;
    goto l_FORCEPEG;
  }

  throw Error("LocalFit","Code breach");

//////////////////////////////////////////////////
 l_BLANKDEPEG: {
    //printf("BLANKDEPEG %Li %Li %Li\n", t_stream, t_limit, t0 - tau + t_blankdepeg);
    if (t_stream >= t_limit)
      return BLANKDEPEG;
    if (t_stream >= t0 - tau + t_blankdepeg)
      goto l_DEPEGGING;
    if (usenegv) {
      int dt = t_stream - t0;
      int dt2 = dt*dt;
      int dt3 = dt*dt2;
      raw_t y = source[t_stream]
        - raw_t(alpha0 + alpha1*dt + alpha2*dt2 + alpha3*dt3);
      if ((y<0) != negv) {
        goto l_DEPEGGING;
      }
    }
    dest[t_stream] = 0;
    t_stream++;
    if (ispegged(source[t_stream+tau+t_ahead])) {
      t0 = t_stream-1;
      calc_X3();
      calc_alpha0123();
      goto l_PEGGING;
    } 
    if (t_stream>t0) {
      update_X012();
    }
    goto l_BLANKDEPEG;
  }

  throw Error("LocalFit","Code breach");

//////////////////////////////////////////////////
 l_DEPEGGING: {
    if (t_stream >= t_limit)
      return DEPEGGING;
    if (t_stream >= t0) {
      goto l_OK;
    }
    int_t dt = t_stream-t0;
    int_t dt2 = dt*dt;
    int_t dt3 = dt*dt2;
    dest[t_stream] = source[t_stream]
      - raw_t(alpha0 + alpha1*dt + alpha2*dt2 + alpha3*dt3);
    t_stream++;
    goto l_DEPEGGING;
  }

  throw Error("LocalFit","Code breach");

//////////////////////////////////////////////////
 l_PEGGING: {
    if (t_stream>=t_limit)
      return PEGGING;
    if (t_stream >= t0+tau) {
      t_peg = t_stream;
      goto l_PEGGED;
    }
    int_t dt = t_stream-t0;
    int_t dt2=dt*dt;
    int_t dt3=dt*dt2;
    dest[t_stream] = source[t_stream]
      - raw_t(alpha0 + alpha1*dt + alpha2*dt2 + alpha3*dt3);
    t_stream++;
    goto l_PEGGING;
  }

  throw Error("LocalFit","Code breach");

//////////////////////////////////////////////////
 l_OK: {
    if (t_stream>=t_limit)
      return OK;
    calc_alpha0();
    dest[t_stream] = source[t_stream] - raw_t(alpha0);
    t_stream++;
    if (ispegged(source[t_stream+tau+t_ahead])) {
      t0=t_stream-1;
      calc_X3();
      calc_alpha0123();
      goto l_PEGGING;
    } 
    update_X012();
    goto l_OK;
  }

  throw Error("LocalFit","Code breach");

//////////////////////////////////////////////////
}

void LocalFit::calc_X012() {
  X0 = X1 = X2 = 0;
  for (int_t t=-tau; t<=tau; t++) {
    int_t t2 = t*t;
    real_t y = source[t0+t];
    X0 += y;
    X1 += t*y;
    X2 += t2*y;
  }
}

void LocalFit::calc_X3() {
  X3 = 0;
  for (int_t t=-tau; t<=tau; t++) {
    int_t t3 = t*t*t;
    real_t y = source[t0+t];
    X3 += t3*y;
  }
}

void LocalFit::update_X0123() {
  // based on calc dd 9/7/01 - do recheck!
  real_t y_new = source[t0+tau];
  real_t y_old = source[t0-tau-1];
  X0 += y_new - y_old;
  X1 += tau_plus_1*y_new - minus_tau*y_old - X0;
  X2 += tau_plus_1_squared*y_new - minus_tau_squared*y_old - X0 - 2*X1;
  X3 += tau_plus_1_cubed*y_new - minus_tau_cubed*y_old - X0 - 3*X1 - 3*X2;
}

void LocalFit::calc_alpha0123() {
  real_t fact02 = 1./(T0*T4-T2*T2);
  alpha0 = fact02*(T4*X0 - T2*X2);
  alpha2 = fact02*(T0*X2 - T2*X0);
#if THIRDORDER
  real_t fact13 = 1./(T2*T6-T4*T4);
  alpha1 = fact13*(T6*X1 - T4*X3);
  alpha3 = fact13*(T2*X3 - T4*X1);
#else
  alpha1 = real_t(X1)/T2;
  alpha3 = 0;
#endif
}

//--------------------------------------------------------------------
// debug
//

void LocalFit::report() {
  Dbg() << "state = "
	<< (state==OK?"OK":
	    state==PEGGING?"PEGGING":
	    state==PEGGED?"PEGGED":
	    state==TOOPOOR?"TOOPOOR":
	    state==DEPEGGING?"DEPEGGING":
	    state==FORCEPEG?"FORCEPEG":
	    state==BLANKDEPEG?"BLANKDEP":
	    "???")
	<< " t_stream=" << t_stream 
	<< " t0=" << t0
	<< " y[ts]=" << source[t_stream]
	<< " alpha=" << alpha0 << " " << alpha1
	<< " " << alpha2 << " " << alpha3
	<< " X=" << X0 << " " << X1
	<< " " << X2 << " " << X3
	<< " y_thr=" << y_threshold
	<< " my_thr=" << my_thresh;
}

void LocalFit::inirep() {
  Dbg() << "tau=" << tau
	<< " T0=" << T0
	<< " T2=" << T2
	<< " T4=" << T4
	<< " T6=" << T6
	<< " t_blank=" << t_blankdepeg
	<< " t_ahead=" << t_ahead
	<< " t_chi2=" << t_chi2
	<< " rail1=" << rail1
	<< " rail2=" << rail2;
}

int_t LocalFit::requireddelay() const {
  return 2*tau + t_ahead + t_chi2 + t_blankdepeg + 10;
}
