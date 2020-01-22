/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2016 Open Ephys

    ------------------------------------------------------------------

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef SALPAPROCESSOR_H_INCLUDED
#define SALPAPROCESSOR_H_INCLUDED

#ifdef _WIN32
#include <Windows.h>
#endif

#include <ProcessorHeaders.h>
#include "NoiseLevels.h"

/**
    This class serves as a template for creating new processors.

    If this were a real processor, this comment section would be used to
    describe the processor's function.

    @see GenericProcessor
*/
class SalpaProcessor : public GenericProcessor {
  enum {
    PARAM_V_ZERO,
    PARAM_V_NEG_RAIL,
    PARAM_V_POS_RAIL,
    PARAM_N_TOOPOOR,
    PARAM_TAU,
    PARAM_T_AHEAD,
    PARAM_T_BLANKDUR,
    PARAM_T_POTBLANK,
    PARAM_T_ASYM,
    PARAM_ABSTHR,
    PARAM_RELTHR,
    PARAM_USEABSTHR,
    PARAM_EVENTCHANNEL,
  };
public:
  SalpaProcessor();
  ~SalpaProcessor();
  bool hasEditor() const { return true; }
  AudioProcessorEditor *createEditor() override;
  void process(AudioSampleBuffer &buffer) override;
  void setParameter(int parameterIndex, float newValue) override;
  void forgetTraining();
  virtual void saveCustomParametersToXml(XmlElement *parentElement) override;
  virtual void loadCustomParametersFromXml() override;
private:
  // parameters
  raw_t v_zero;
  raw_t v_neg_rail, v_pos_rail;
  int n_toopoor;
  int tau;
  int t_ahead, t_blankdur, t_potblank, t_asym;
  raw_t absthr;
  float relthr;
  bool useabsthr;
  int eventchannel;
  // helpers
  std::vector<class LocalFit *> fitters;
  NoiseLevels noise;
private:
  void createFitters(int nChannels);
  void dropFitters();
  void train(AudioSampleBuffer &buffer);
  //
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SalpaProcessor);
};


#endif  // SALPAPROCESSOR_H_INCLUDED
