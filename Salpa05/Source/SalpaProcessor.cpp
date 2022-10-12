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
const int NTHREADS = 8; // or 16

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
                                  relthr(3.5),
                                  eventchannel(-1),
                                  delay(0),
                                  threadpool(NTHREADS) {
  eventChannelPtr = 0;
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
  editor->updateParameterButtons(idx); // ?
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
  mustrebuild = true;
}

void SalpaProcessor::train(AudioSampleBuffer &buffer) {
  printf("SALPA: train\n");
  noise.train(buffer, getNumSamples(0));
  if (noise.chunks() >= TRAINCHUNKS) {
    //printf("SALPA: done training\n");
    noise.makeready();
    //for (int n=0; n<fitters.size(); n++)
    //  printf("noise[%i] = %g\n", n, noise.std(n));
    if (!useabsthr || absthr==0)
      for (int n=0; n<fitters.size(); n++)
        fitters[n]->setthreshold(relthr * noise.std(n));
  } else {
    //    printf("SALPA: more training required (%i)\n", noise.chunks());
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
  fitters.clear();
  inbufs.clear();
  outbufs.clear();
}

void SalpaProcessor::createFitters(int nChannels) {
  mustrebuild = false;
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
    fitters[c]->setthreshold((useabsthr && absthr>0) ? absthr : relthr * noise.std(c));
    delay = fitters[c]->requireddelay(); // will be all the same
    if (tau + t_potblank > delay)
      delay = tau + t_potblank;
  }
  //for (int c=0; c<nChannels; c++)
  // inbufs[c]->donewriting(delay);
}

void SalpaProcessor::prepareToPlay(double sampleRate, int estimatedSamplesPerBlock) {
  dropFitters();
}

void SalpaProcessor::process(AudioSampleBuffer &buffer) {
  juce::int64 startTs = getTimestamp(0);
  //  printf("SALPA: process: %Li\n", startTs);
  t0 = startTs;
  if (eventchannel>=0) 
    checkForEvents();
  if (!noise.isready())
    train(buffer);

  int nChannels = buffer.getNumChannels();
  if (nChannels<=0) {
    printf("SALPA: no channels\n");
    return;
  }
  int nSamples = getNumSamples(0);
  //  printf("t0=%li inbuf@%li outbuf@%li\n", t0, inbufs[0]->latest(), outbufs[0]->latest());

  if (mustrebuild || nChannels != fitters.size()) {
    createFitters(nChannels);
    // this must be the first part of the run or parameters have changed
    for (int c=0; c<nChannels; c++) {
      inbufs[c]->reset(startTs);
      outbufs[c]->reset(startTs);
      fitters[c]->reset(startTs);
      outbufs[c]->fill(v_zero);
    }
    fitters[0]->inirep();
  }

  for (int c=0; c<nChannels; c++) {
    float const *samplePtr = buffer.getReadPointer(c, 0);
    CyclBuf<raw_t> &inbuf(*inbufs[c]);
    timeref_t t0 = inbuf.latest();
    for (int n=0; n<nSamples; n++)
      inbuf[t0+n] = samplePtr[n];
    inbuf.donewriting(nSamples);
  }

  timeref_t tlim = inbufs[0]->latest();
  if (tlim < delay)
    tlim = 0;
  else
    tlim -= delay;

  int step = nChannels / NTHREADS;
  if (step*NTHREADS < nChannels)
    step++;

  if (eventchannel >= 0) {
    while (!forcestarts.empty()) {
      timeref_t tfrom = forcestarts.front();
      timeref_t tto = forceends.front();
      //printf("forcepeg %li - %li [%li - %li]\n",
      //     tfrom, tto, t0, tlim);
      if (tto <= tlim) {
        for (int c0=0; c0<nChannels; c0+=step) {
          int c1 = c0 + step;
          if (c1>nChannels)
            c1 = nChannels;
          std::packaged_task<void()> task([c0, c1, tfrom, tto, this]() {
                                            for (int c=c0; c<c1; c++)
                                              fitters[c]->forcepeg(tfrom, tto);
                                          });
          threadpool.post(task);
        }
        threadpool.wait();
        forcestarts.pop();
        forceends.pop();
      } else if (tfrom < tlim) {
        for (int c0=0; c0<nChannels; c0+=step) {
          int c1 = c0 + step;
          if (c1>nChannels)
            c1 = nChannels;
          std::packaged_task<void()> task([c0, c1, tfrom, tlim, this]() {
                                            for (int c=c0; c<c1; c++)
                                              fitters[c]->forcepeg(tfrom, tlim);
                                          });
          threadpool.post(task);
        }
        threadpool.wait();
        forcestarts.front() = tlim;
        // leave some of the forced peg to next cycle
        break;
      } else {
        // leave all ofr the forced peg to the next cycle
        break;
      }
    }
  }
  for (int c0=0; c0<nChannels; c0+=step) {
    int c1 = c0 + step;
    if (c1>nChannels)
      c1 = nChannels;
    std::packaged_task<void()> task([c0, c1, tlim, this]() {
                                      for (int c=c0; c<c1; c++)
                                              fitters[c]->process(tlim);
                                    });
    threadpool.post(task);
  }
  threadpool.wait();

  for (int c=0; c<nChannels; c++) {
    float *samplePtr = buffer.getWritePointer(c, 0);
    CyclBuf<raw_t> &outbuf(*outbufs[c]);
    for (int n=0; n<nSamples; n++)
      samplePtr[n] = outbuf[t0 - delay + n];
  }

  while (!futureEvents.empty()) {
    auto evt = futureEvents.front();
    if (evt.t < t0 + nSamples) {
        MetaDataValueArray mdArray;
        TTLEventPtr event = TTLEvent::createTTLEvent(eventChannelPtr,
                                                        evt.t,
                                                        &evt.v,
                                                        sizeof(juce::uint8),
                                                        mdArray,
                                                        evt.c);
        addEvent(eventChannelPtr, event, evt.t - t0);
        futureEvents.pop();
    } else {
      break;
    }
  }

  //  fitters[0]->report();
}

void SalpaProcessor::handleEvent(EventChannel const *eventInfo,
                                 MidiMessage const &event,
                                 int samplePosition) {
  if (Event::getEventType(event) == EventChannel::TTL) {
    TTLEventPtr ttl = TTLEvent::deserializeFromMessage(event, eventInfo);
    int id = ttl->getState() ? 1 : 0;
    int ch = ttl->getChannel();
    int t = samplePosition;
    //printf("Salpa: event id %i channel %i time %i / %i\n", id, ch, t, eventchannel);
    if (id && ch==eventchannel) {
      forcestarts.push(t + t0);
      forceends.push(t + t0 + t_potblank);

      if (eventChannelPtr) {
	juce::uint8 chmsk = juce::uint8(2<<ch);
        futureEvents.push(EventPrep{t0 + t + delay, chmsk, ch+1});
        futureEvents.push(EventPrep{t0 + t + t_potblank + delay, 0, ch+1});
      }
    }
  }
}


void SalpaProcessor::createEventChannels() {
  DataChannel const *in = getDataChannel(0);
  if (!in) {
    eventChannelPtr = nullptr;
    printf("No input - no event channel created\n");
    return;
  }

  //  float sampleRate = in->getSampleRate();
  EventChannel *chan = new EventChannel(EventChannel::TTL, 8, 1,
                                        in, this);
  chan->setName("SALPA events");
  chan->setDescription("Delayed event");
  chan->setIdentifier("salpa.event");

  eventChannelPtr = eventChannelArray.add(chan);
}

void SalpaProcessor::saveMyParameters(XmlElement *xml) const {
  XmlElement *mainNode = xml->createNewChildElement("VALUES");

  auto saveParameter = [this, mainNode](int idx, float value) {
    auto parameter = getParameterObject(idx);
    mainNode->setAttribute(parameter->getName(), value);
  };

  saveParameter(PARAM_V_NEG_RAIL, v_neg_rail);
  saveParameter(PARAM_V_POS_RAIL, v_pos_rail);
  saveParameter(PARAM_T_POTBLANK, t_potblank);
  saveParameter(PARAM_T_BLANKDUR, t_blankdur);
  saveParameter(PARAM_N_TOOPOOR, n_toopoor);
  saveParameter(PARAM_T_AHEAD, t_ahead);
  saveParameter(PARAM_TAU, tau);
  saveParameter(PARAM_RELTHR, relthr);
  saveParameter(PARAM_ABSTHR, absthr);
  saveParameter(PARAM_USEABSTHR, useabsthr);
  saveParameter(PARAM_T_ASYM, t_asym);
  saveParameter(PARAM_EVENTCHANNEL, eventchannel);
  saveParameter(PARAM_V_ZERO, v_zero);
  printf("Salpa - save my parameters\n");
}

void SalpaProcessor::loadMyParameters(XmlElement *xml) {
  bool ok = false;
  forEachXmlChildElementWithTagName(*xml, mainNode, "VALUES") {
    auto loadParameter = [this, mainNode](int idx) {
      auto parameter = getParameterObject(idx);
      return mainNode->getDoubleAttribute(parameter->getName());
    };
    ok = true;
    for (int k=0; k<PARAMETER_COUNT; k++) {
      auto paramobj = getParameterObject(k);
      float value = mainNode->getDoubleAttribute(paramobj->getName());
      setParameter(k, value);
    }
    //    printf("Salpa - load my parameters\n");
  }
  if (!ok) 
    printf("SALPAPROCESSOR - NO XML CONTENT\n");
}
