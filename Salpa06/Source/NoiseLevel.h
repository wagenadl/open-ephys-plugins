/* common/NoiseLevels.H: part of meabench, an MEA recording and analysis tool
** Copyright (C) 2000-2002  Daniel Wagenaar (wagenaar@caltech.edu)
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

// NoiseLevels.H

#ifndef NOISELEVELS_H

#define NOISELEVELS_H

#include "MedianVariance.h"

#include <math.h>
#include <vector>


class NoiseLevel {
public:
  static const int MINCHUNKS=5;
public:
  NoiseLevel() { reset(); }
  float mean() const { return ready ? means : 0; }
  float var() const { return ready ? vars : 1; }
  float std() const { return ready ? stds : 1; }
  void reset() {
    mv.reset();
    ready=false;
  }
  void setnotready() {
    ready=false;
  }
  void train(float const *samplePtr, int nSamples) {
    for (int i=0; i<nSamples; i++)
      mv.addexample(samplePtr[i]);
  }
  bool makeready()  {
    if (chunks() < MINCHUNKS) {
      printf("NoiseLevels: Too few chunks to compute meaningful estimates\n");
      return false;
    }
    means=mv.mean();
    vars=mv.var();
    stds=sqrt(vars);
    ready=true;
    return true;
  }
  void force(int hw, double mean, double var) {
    means=mean; vars=var; stds=sqrt(var);
    ready=true;
  }
  int chunks() const { return mv.chunks(); }
  void cleanout() {
    mv.reset();
  }
  bool isready() const { return ready; }
private:
  MedianVariance<float> mv;
  float means;
  float vars;
  float stds;
  bool ready;
};

#endif

