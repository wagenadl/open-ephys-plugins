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
#include <ProcessorHeaders.h>

class NoiseLevels {
public:
  static const int MINCHUNKS=5;
public:
  NoiseLevels() { reset(); }
  float mean(int hw) const { return ready ? means[hw] : 0; }
  float var(int hw) const { return ready ? vars[hw] : 1; }
  float std(int hw) const { return ready ? stds[hw] : 1; }
  float operator[](int hw) const { return std(hw); }
  void reset() {
    mv.clear();
    ready=false;
  }
  void setnotready() {
    ready=false;
  }
  void train(AudioSampleBuffer &buffer) {
    int nChannels = buffer.getNumChannels();
    if (nChannels != mv.size()) {
      mv.resize(nChannels);
      for (int c=0; c<nChannels; c++)
        mv[c].reset();
    }
    for (int c=0; c<nChannels; c++) {
      int nSamples = getNumSamples(c);
      float *samplePtr = buffer.getReadPointer(c, 0);
      MedianVariance &mv1(mv[c]);
      for (int i=0; i<nSamples; i++)
        mv1.addexample(samplePtr[i]);
    }
  }
  bool makeready()  {
    if (chunks() < MINCHUNKS) {
      printf("NoiseLevels: Too few chunks to compute meaningful estimates\n");
      return false;
    }
    means.resize(mv.size());
    vars.resize(mv.size());
    stds.resize(mv.size());
    for (int hw=0; hw<mv.size(); hw++) {
      means[hw]=mv[hw].mean();
      vars[hw]=mv[hw].var();
      stds[hw]=sqrt(vars[hw]);
    }
    ready=true;
    return true;
  }
  void force(int hw, double mean, double var) {
    means[hw]=mean; vars[hw]=var; stds[hw]=sqrt(var);
    ready=true;
  }
  int chunks() const { return mv[0].chunks(); }
  void cleanout() {
    for (int hw=0; hw<mv.size(); hw++)
      mv[hw].reset();
  }
  bool isready() const { return ready; }

  void report() {
    report("Means",means);
    report("RMS",stds);
    fprintf(stderr,"All values are digital units.\n");
  }
  void report(char const *label, float *data) {
    fprintf(stderr,"%s:\n",label);
    fprintf(stderr,"%4s","");
    for (int col=0; col<8; col++)
      fprintf(stderr,"%8i",col+1);
    fprintf(stderr,"\n");
    
    for (int row=0; row<8; row++) {
      fprintf(stderr,"%4i",row+1);
      for (int col=0; col<8; col++) {
	int hw = cr2hw(col,row);
	string r = Sprintf("%.1f",data[hw]);
	if (r.size()>7)
	  r = r.substr(0,7);
	fprintf(stderr,"%8s",r.c_str());
      }
      fprintf(stderr,"\n");
    }
  }

private:
  std::vector<MedianVariance<double>> mv;
  std::vector<float> means;
  std::vector<float> vars;
  std::vector<float> stds;
  bool ready;
};

#endif
