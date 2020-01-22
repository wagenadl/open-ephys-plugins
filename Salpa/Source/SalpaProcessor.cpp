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


#include <stdio.h>

#include "SalpaProcessor.h"
#include "SalpaProcessorEditor.h"
#include "LocalFit.h"

const int FREQKHZ = 30; // whatever
const int TRAINCHUNKS = 20;
const int LOGBUFSIZE = 12; // BUFSIZE = 1<<12 = 4096 samples

SalpaProcessor::SalpaProcessor(): GenericProcessor("SALPA"),
                                  v_zero(0),
                                  v_neg_rail(-4000), v_pos_rail(4000),
                                  n_toopoor(5),
                                  tau(20),
                                  t_ahead(5),
                                  t_blankdur(5),
                                  t_potblank(5),
                                  t_asym(15),
                                  absthr(0),
                                  relthr(3),
                                  eventchannel(-1),
                                  delay(0) {
  setProcessorType (PROCESSOR_TYPE_FILTER);

  // Open Ephys Plugin Generator will insert generated code for parameters here. Don't edit this section.
  //[OPENEPHYS_PARAMETERS_SECTION_BEGIN]
    // Parameter 0 code
    auto parameter0 = new Parameter ("negativeRail", -32767, -1, -3000, 0);
    parameters.add (parameter0);

    // Parameter 1 code
    auto parameter1 = new Parameter ("positiveRail", 1, 32767, 2000, 1);
    parameters.add (parameter1);

    // Parameter 2 code
    auto parameter2 = new Parameter ("t_potblank", 1, 100, 15, 2);
    parameters.add (parameter2);

    // Parameter 3 code
    auto parameter3 = new Parameter ("t_blank", 1, 100, 5, 3);
    parameters.add (parameter3);

    // Parameter 4 code
    auto parameter4 = new Parameter ("n_tooPoor", 0, 100, 5, 4);
    parameters.add (parameter4);

    // Parameter 5 code
    auto parameter5 = new Parameter ("t_ahead", 0, 100, 5, 5);
    parameters.add (parameter5);

    // Parameter 6 code
    auto parameter6 = new Parameter ("tau", 1, 1000, 30, 6);
    parameters.add (parameter6);

    // Parameter 7 code
    auto parameter7 = new Parameter ("relThresh", 1, 100, 3, 7);
    parameters.add (parameter7);

    // Parameter 8 code
    auto parameter8 = new Parameter ("absThresh", 0, 32700, 0, 8);
    parameters.add (parameter8);

    // Parameter 9 code
    auto parameter9 = new Parameter ("useAbsThresh", false, 9);
    parameters.add (parameter9);

    // Parameter 10 code
    auto parameter10 = new Parameter ("t_asym", 0, 1000, 0, 10);
    parameters.add (parameter10);

    // Parameter 11 code
    auto parameter11 = new Parameter ("eventChannel", "eventChannel", -1, 7, 0, 11);
    parameters.add (parameter11);

    // Parameter 12 code
    auto parameter12 = new Parameter ("v_zero", -32700, 32700, 0, 12);
    parameters.add (parameter12);


  //[OPENEPHYS_PARAMETERS_SECTION_END]

  //Without a custom editor, generic parameter controls can be added
  //parameters.add(Parameter("thresh", 0.0, 500.0, 200.0, 0));
}

SalpaProcessor::~SalpaProcessor() {
  dropFitters();
}

/**
   If the processor uses a custom editor, this method must be present.
*/
AudioProcessorEditor* SalpaProcessor::createEditor() {
  editor = new SalpaProcessorEditor(this, false);
  return editor;
}

void SalpaProcessor::setParameter(int idx, float val) {
  printf("salpa set param %i %g\n", idx, val);
  GenericProcessor::setParameter(idx, val);
  editor->updateParameterButtons(idx);
  switch (idx) {
  case PARAM_V_ZERO:
    v_zero = val;
    break;
  case PARAM_V_NEG_RAIL:
    v_neg_rail = val;
    break;
  case PARAM_V_POS_RAIL:
    v_pos_rail = val;
    break;
  case PARAM_N_TOOPOOR:
    n_toopoor = val;
    break;
  case PARAM_TAU:
    tau = val;
    break;
  case PARAM_T_AHEAD:
    t_ahead = val;
    break;
  case PARAM_T_BLANKDUR:
    t_blankdur = val;
    break;
  case PARAM_T_POTBLANK:
    t_potblank = val;
    break;
  case PARAM_T_ASYM:
    t_asym = val;
    break;
  case PARAM_ABSTHR:
    absthr = val;
    break;
  case PARAM_RELTHR:
    relthr = val;
    break;
  case PARAM_USEABSTHR:
    useabsthr = val;
    break;
  case PARAM_EVENTCHANNEL:
    eventchannel = val;
    break;
  }
  dropFitters();
}

void SalpaProcessor::train(AudioSampleBuffer &buffer) {
  printf("SALPA: train\n");
  noise.train(buffer, getNumSamples(0));
  if (noise.chunks() >= TRAINCHUNKS) {
    printf("SALPA: done training\n");
    noise.makeready();
    for (int n=0; n<fitters.size(); n++)
      printf("noise[%i] = %g\n", n, noise.std(n));
    if (!useabsthr)
      for (int n=0; n<fitters.size(); n++)
        fitters[n]->setthreshold(relthr * noise.std(n));
  } else {
    printf("SALPA: more training required (%i)\n", noise.chunks());
  }
}

void SalpaProcessor::forgetTraining() {
  noise.reset();
}

void SalpaProcessor::dropFitters() {
  for (auto f: fitters)
    delete f;
  for (auto b: inbufs)
    delete b;
  for (auto b: outbufs)
    delete b;
}

void SalpaProcessor::createFitters(int nChannels) {
  dropFitters();
  fitters.resize(nChannels);
  inbufs.resize(nChannels);
  outbufs.resize(nChannels);
  delay = 0;
  for (int c=0; c<nChannels; c++) {
    inbufs[c] = new CyclBuf<raw_t>(LOGBUFSIZE);
    outbufs[c] = new CyclBuf<raw_t>(LOGBUFSIZE);
    fitters[c] = new LocalFit(*inbufs[c], *outbufs[c],
                              0, 0, tau,
                              t_blankdur, t_ahead,
                              t_asym);
    fitters[c]->setrail(v_neg_rail, v_pos_rail);
    fitters[c]->setthreshold(useabsthr ? absthr : relthr * noise.std(c));
    delay = fitters[c]->requireddelay(); // will be all the same
  }
}

void SalpaProcessor::process(AudioSampleBuffer &buffer) {
  printf("SALPA: process\n");
  if (!noise.isready())
    train(buffer);

  int nChannels = buffer.getNumChannels();
  if (nChannels != fitters.size()) {
    createFitters(nChannels);
    // this must be the first part of the run
    for (int c=0; c<nChannels; c++)
      outbufs[c]->fill(v_zero);
    fitters[0]->inirep();
  }

  for (int c=0; c<nChannels; c++) {
    int nSamples = getNumSamples(c);
    float *samplePtr = buffer.getWritePointer(c, 0);
    CyclBuf<raw_t> &inbuf(*inbufs[c]);
    timeref_t t0 = inbuf.latest();
    for (int n=0; n<nSamples; n++)
      inbuf[t0+n] = samplePtr[n];
    inbuf.donewriting(nSamples);
    if (inbuf.latest() > delay)
      fitters[c]->process(inbuf.latest() - delay);
    CyclBuf<raw_t> &outbuf(*outbufs[c]);
    for (int n=0; n<nSamples; n++)
      samplePtr[n] = outbuf[t0 - delay + n];
  }
  fitters[0]->report();
}

void SalpaProcessor::saveCustomParametersToXml (XmlElement* parentElement) {
  XmlElement *mainNode = parentElement->createNewChildElement("SalpaProcessor");
  mainNode->setAttribute("numParameters", getNumParameters());

  // Open Ephys Plugin Generator will insert generated code to save parameters here. Don't edit this section.
  //[OPENEPHYS_PARAMETERS_SAVE_SECTION_BEGIN]
  for (int i = 0; i < getNumParameters(); ++i)
    {
      XmlElement* parameterNode = mainNode->createNewChildElement ("Parameter");

      auto parameter = getParameterObject(i);
      parameterNode->setAttribute ("name", parameter->getName());
      parameterNode->setAttribute ("type", parameter->getParameterTypeString());

      auto parameterValue = getParameterVar (i, currentChannel);

      if (parameter->isBoolean())
        parameterNode->setAttribute ("value", (int)parameterValue);
      else if (parameter->isContinuous() || parameter->isDiscrete() || parameter->isNumerical())
        parameterNode->setAttribute ("value", (double)parameterValue);
    }
  //[OPENEPHYS_PARAMETERS_SAVE_SECTION_END]
}


void SalpaProcessor::loadCustomParametersFromXml() {
  if (parametersAsXml == nullptr) // prevent double-loading
    return;

  // use parametersAsXml to restore state

  // Open Ephys Plugin Generator will insert generated code to load parameters here. Don't edit this section.
  //[OPENEPHYS_PARAMETERS_LOAD_SECTION_BEGIN]
  forEachXmlChildElement (*parametersAsXml, mainNode)
    {
      if (mainNode->hasTagName ("SalpaProcessor"))
        {
          int parameterIdx = -1;

          forEachXmlChildElement (*mainNode, parameterNode)
            {
              if (parameterNode->hasTagName ("Parameter"))
                {
                  ++parameterIdx;

                  String parameterType = parameterNode->getStringAttribute ("type");
                  if (parameterType == "Boolean")
                    setParameter (parameterIdx, parameterNode->getBoolAttribute ("value"));
                  else if (parameterType == "Continuous" || parameterType == "Numerical")
                    setParameter (parameterIdx, parameterNode->getDoubleAttribute ("value"));
                  else if (parameterType == "Discrete")
                    setParameter (parameterIdx, parameterNode->getIntAttribute ("value"));
                }
            }
        }
    }
  //[OPENEPHYS_PARAMETERS_LOAD_SECTION_END]
}
