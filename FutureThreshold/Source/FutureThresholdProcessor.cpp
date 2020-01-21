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

#include "FutureThresholdProcessor.h"
#include "FutureThresholdProcessorEditor.h"

//If the processor uses a custom editor, it needs its header to instantiate it
//#include "ExampleEditor.h"


FutureThresholdProcessor::FutureThresholdProcessor():
  GenericProcessor("FutureThreshold"), //, threshold(200.0), state(true)
  input_channel(-1),
  output_channel(0),
  trigger_threshold(10000),
  reset_threshold(5000),
  future_samps(0),
  triggered(false),
  eventChannelPtr(nullptr)
{
  setProcessorType (PROCESSOR_TYPE_FILTER);

  // Open Ephys Plugin Generator will insert generated code for parameters here. Don't edit this section.
  //[OPENEPHYS_PARAMETERS_SECTION_BEGIN]
    // Parameter 0 code
    Array<var> parameter0PossibleValues {0};
    auto parameter0 = new Parameter ("adcChannel", parameter0PossibleValues, 0, 0);
    parameters.add (parameter0);

    // Parameter 1 code
    Array<var> parameter1PossibleValues {0};
    auto parameter1 = new Parameter ("eventChannel", parameter1PossibleValues, 0, 1);
    parameters.add (parameter1);

    // Parameter 2 code
    auto parameter2 = new Parameter ("triggerThreshold", "triggerThreshold", -32767, 32767, 0, 2);
    parameters.add (parameter2);

    // Parameter 3 code
    auto parameter3 = new Parameter ("resetThreshold", "resetThreshold", -32767, 32767, 0, 3);
    parameters.add (parameter3);

    // Parameter 4 code
    auto parameter4 = new Parameter ("delayMS", 0, 100, 0, 4);
    parameters.add (parameter4);


  //[OPENEPHYS_PARAMETERS_SECTION_END]

  //Without a custom editor, generic parameter controls can be added
  //parameters.add(Parameter("thresh", 0.0, 500.0, 200.0, 0));
}


FutureThresholdProcessor::~FutureThresholdProcessor() {
}


/**
   If the processor uses a custom editor, this method must be present.
*/
AudioProcessorEditor* FutureThresholdProcessor::createEditor() {
  editor = new FutureThresholdProcessorEditor (this, false);
  return editor;
}


void FutureThresholdProcessor::setParameter(int parameterIndex, float newValue)
{
  GenericProcessor::setParameter (parameterIndex, newValue);
  editor->updateParameterButtons (parameterIndex);

  switch (parameterIndex) {
  case PARAM_INPUT_CHANNEL:
    input_channel = newValue;
    printf("input_channel = %i\n", input_channel);
    break;
  case PARAM_OUTPUT_CHANNEL:
    output_channel = newValue;
    printf("output_channel = %i\n", output_channel);
    break;
  case PARAM_TRIGGER_THRESHOLD:
    trigger_threshold = newValue;
    printf("trigger_threshold = %f\n", trigger_threshold);
    break;
  case PARAM_RESET_THRESHOLD:
    reset_threshold = newValue;
    printf("reset_threshold = %f\n", reset_threshold);
    break;
  case PARAM_DELAY_SAMPS:
    future_samps = newValue;
    circbufs.reset();
    circbufs.unget(future_samps);
    printf("delay = %i samples\n", future_samps);
    break;
  }
}


void FutureThresholdProcessor::createEventChannels() {
  DataChannel const *in = getDataChannel(input_channel);
  if (!in) {
    eventChannelPtr = nullptr;
    printf("No input - no event channel created\n");
    return;
  }

  float sampleRate = in->getSampleRate();
  EventChannel *chan = new EventChannel(EventChannel::TTL, 8, 1,
                                        sampleRate, this);
  chan->setName("Schmitt trigger output");
  chan->setDescription("Triggers whenever the input signal crosses"
                       " a voltage threshold.");
  chan->setIdentifier("schmitt.event");

  eventChannelPtr = eventChannelArray.add(chan);
}


void FutureThresholdProcessor::process(AudioSampleBuffer &buffer) {
  if (input_channel < 0
      || input_channel >= buffer.getNumChannels()
      || !eventChannelPtr) {
    printf("Cannot process\n");
    //    jassertfalse;
    return;
  }

  int nSamples = /*buffer.getNumSamples(); //*/ getNumSamples(input_channel);
  printf("nsamples = %i [%i] x %i\n", nSamples, getNumSamples(input_channel), buffer.getNumChannels());
  //         addEvents(events, TTL, n);
  float const *rp = buffer.getReadPointer(input_channel);
  juce::int64 startTs = getTimestamp(input_channel);

  int sgn = trigger_threshold > reset_threshold ? 1 : -1;
  float thr_rst = sgn*reset_threshold;
  float thr_trg = sgn*trigger_threshold;
  for (int i=0; i<nSamples; i++) {
    float val = rp[i];
    if (triggered) {
      if (sgn*val <= thr_rst) {
        // create reset event
        MetaDataValueArray mdArray;
        // MetaDataValue *mdv_time
        //   = new MetaDataValue(*eventMetaDataDescriptors[0]);
        // mdv_time->setValue(startTs + i);
        // mdArray.add(mdv_time);
        juce::uint8 ttlDataOff = 0;
        juce::int64 eventTsOff = startTs + i;
        TTLEventPtr eventOff = TTLEvent::createTTLEvent(eventChannelPtr,
                                                        eventTsOff,
                                                        &ttlDataOff,
                                                        sizeof(juce::uint8),
                                                        mdArray,
                                                        output_channel);
        addEvent(eventChannelPtr, eventOff, i);
        triggered = false;
      }
    } else {
      if (sgn*val >= thr_trg) {
        // create trigger event
        printf("TRIGGER %Li\n", startTs + i);
        MetaDataValueArray mdArray;
        // MetaDataValue *mdv_time
        //   = new MetaDataValue(*eventMetaDataDescriptors[0]);
        // mdv_time->setValue(startTs + i);
        // mdArray.add(mdv_time);
        juce::uint8 ttlDataOn = 1 << output_channel;
        juce::int64 eventTsOn = startTs + i;
        TTLEventPtr eventOn = TTLEvent::createTTLEvent(eventChannelPtr,
                                                       eventTsOn,
                                                       &ttlDataOn,
                                                       sizeof(juce::uint8),
                                                       mdArray,
                                                       output_channel);
        addEvent(eventChannelPtr, eventOn, i);
        triggered  = true;
      }
    }
  }

  if (future_samps) {
    int nCh = buffer.getNumChannels();
    for (int c=0; c<nCh; c++) {
      float *rp = buffer.getWritePointer(c);
      int nSam = getNumSamples(c); //*/ buffer.getNumSamples();
      CircularBuffer *buf = circbufs.channel(c);
      for (int n=0; n<nSam; n++) {
        buf->put(rp[n]);
        rp[n] = buf->get();
      }
    }
  }
}


void FutureThresholdProcessor::saveCustomParametersToXml (XmlElement* parentElement)
{
  XmlElement* mainNode = parentElement->createNewChildElement ("FutureThresholdProcessor");
  mainNode->setAttribute ("numParameters", getNumParameters());

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


void FutureThresholdProcessor::loadCustomParametersFromXml()
{
  if (parametersAsXml == nullptr) // prevent double-loading
    return;

  // use parametersAsXml to restore state

  // Open Ephys Plugin Generator will insert generated code to load parameters here. Don't edit this section.
  //[OPENEPHYS_PARAMETERS_LOAD_SECTION_BEGIN]
  forEachXmlChildElement (*parametersAsXml, mainNode)
    {
      if (mainNode->hasTagName ("FutureThresholdProcessor"))
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

void FutureThresholdProcessor::prepareToPlay(double fs_hz, int estspb) {
  printf("prepare to play %g / %i\n", fs_hz, estspb);
  printf("  num channels %i\n", getNumInputs());
  printf("  fs %g\n", getSampleRate());
}
