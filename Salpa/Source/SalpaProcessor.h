/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2020 Daniel Wagenaar
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
#include "LocalFit.h"
#include "CyclBuf.h"
#include <queue>

/**
    This class serves as a template for creating new processors.

    If this were a real processor, this comment section would be used to
    describe the processor's function.

    @see GenericProcessor
*/
class SalpaProcessor : public GenericProcessor {
public:
  enum { // must match jucer
    PARAM_V_NEG_RAIL=0,
    PARAM_V_POS_RAIL=1,
    PARAM_T_POTBLANK=2,
    PARAM_T_BLANKDUR=3,
    PARAM_N_TOOPOOR=4,
    PARAM_T_AHEAD=5,
    PARAM_TAU=6,
    PARAM_RELTHR=7,
    PARAM_ABSTHR=8,
    PARAM_USEABSTHR=9,
    PARAM_T_ASYM=10,
    PARAM_EVENTCHANNEL=11,
    PARAM_V_ZERO=12,
  };
public:
  SalpaProcessor();
  ~SalpaProcessor();
  bool hasEditor() const { return true; }
  AudioProcessorEditor *createEditor() override;
  void process(AudioSampleBuffer &buffer) override;
  void setParameter(int parameterIndex, float newValue) override;
  void forgetTraining();
  void handleEvent(EventChannel const *eventInfo, MidiMessage const &event,
                   int samplePosition) override;
  void createEventChannels() override;
  void prepareToPlay(double sampleRate, int estimatedSamplesPerBlock) override;
private:
  friend class SalpaProcessorEditor;
  friend class SalpaProcessorVisualizer;
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
  std::vector<LocalFit *> fitters;
  std::vector<CyclBuf<raw_t> *> inbufs;
  std::vector<CyclBuf<raw_t> *> outbufs;
  NoiseLevels noise;
  int delay;
  std::queue<timeref_t> forcestarts;
  std::queue<timeref_t> forceends;
  timeref_t t0;
  bool mustrebuild;
  EventChannel *eventChannelPtr;
private:
  void createFitters(int nChannels);
  void dropFitters();
  void train(AudioSampleBuffer &buffer);
  //
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SalpaProcessor);
};


#endif  // SALPAPROCESSOR_H_INCLUDED
