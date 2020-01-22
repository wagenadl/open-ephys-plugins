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

const int TRAINLENGTH = FREQKHZ*5000;



SalpaProcessor::SalpaProcessor(): GenericProcessor("SALPA") {
  setProcessorType (PROCESSOR_TYPE_FILTER);

  // Open Ephys Plugin Generator will insert generated code for parameters here. Don't edit this section.
  //[OPENEPHYS_PARAMETERS_SECTION_BEGIN]
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
  editor = new SalpaProcessorEditor (this, true);

  //std::cout << "Creating editor." << std::endl;

  return editor;
}

void SalpaProcessor::setParameter(int idx, float val) {
  GenericProcessor::setParameter(idx, val);
  editor->updateParameterButtons(idx, val);
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
  noise.train(buffer);
  if (noise.chunks() >= TRAINCHUNKS) {
    noise.makeready();
    if (!useabsthr) 
      for (int n=0; n<fitters.size(); n++)
        fitters[n]->setthreshold(relthr * noise.std(n));
  }
}

void SalpaProcessor::forgetTraining() {
  noise.reset();
}

void SalpaProcessor::dropFitters() {
  for (auto f: fitters):
    delete f;
}   
 
void SalpaProcessor::createFitters(int nChannels) {
  dropFitters();
  fitters.resize(nChannels);
  for (int c=0; c<nChannels; c++) {
    fitters[c] = new LocalFit(tau, t_blankdur, t_ahead, t_asym);
    fitters[c]->setrail(v_neg_rail, v_pos_rail);
    fitters[c]->setthreshold(useabsthr ? absthr : relthr * noise.std(c));
}

void SalpaProcessor::process(AudioSampleBuffer &buffer) {
  if (!noise.isready())
    train(buffer);
  
  int nChannels = buffer.getNumChannels();
  if (nChannels != fitters.size())
    createFitters();
  
  for (int c=0; c<nChannels; c++) {
    int nSamples = getNumSamples(c);
    float *samplePtr = buffer.getWritePointer(c, 0);
    fitters[c]->process(samplePtr, nSamples);
  }
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
