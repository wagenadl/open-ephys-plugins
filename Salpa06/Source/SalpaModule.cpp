// SalpaModule.cpp

#include "SalpaModule.h"

SalpaModule::SalpaModule(): v_zero(0),
                            v_neg_rail(-4000), v_pos_rail(4000),
                            n_toopoor(5),
                            tau(20),
                            t_ahead(5), t_blankdur(5), t_potblank(5),
                            t_asym(15),
                            absthr(0),
                            relthr(3.5),
                            useabsthr(false),
                            eventchannel(-1),
                            threadpool(NTHREADS),
                            outputEventChannel(NULL) {
}

SalpaModule::~SalpaModule() {
}

void SalpaModule::debugParams() {
  std::cerr << "SalpaModule::DebugParams\n";
  bool first = true;
  for (auto &f: fitters) {
    if (first) {
      (f.second)->inirep();
      (f.second)->report();
      first = false;
    }
  }
}

void SalpaModule::createFitters(int64 startsample) {
  dropFitters();
  delay = 0;
  for (auto const &group: groupedchannels) {
    for (uint16 c: group) {
      inbufs[c] = new CyclBuf<raw_t>(LOGBUFSIZE);
      inbufs[c]->reset(startsample);
      outbufs[c] = new CyclBuf<raw_t>(LOGBUFSIZE);
      outbufs[c]->reset(startsample);
      outbufs[c]->fill(v_zero);
      fitters[c] = new LocalFit(*inbufs[c], *outbufs[c],
                                0, 0, tau,
                                t_blankdur, t_ahead,
                                t_asym);
      fitters[c]->setrail(v_neg_rail, v_pos_rail);
      fitters[c]->setthreshold((useabsthr && absthr>0)
                               ? absthr
                               : relthr * noise[c].std());
      fitters[c]->reset(startsample);
      delay = fitters[c]->requireddelay(); // will be all the same
      if (tau + t_potblank > delay)
        delay = tau + t_potblank;
    }
  }
}

std::list<SalpaModule::OutputEvent> SalpaModule::process(
                          AudioBuffer<float> &buffer,
                          int64 startsample, uint32 nsamples) {
  std::list<SalpaModule::OutputEvent> result;  
  if (groupedchannels.empty())
    return result;
  
  if (!noiseready)
    train(buffer, nsamples);
  
  if (fitters.empty()) {
    createFitters(startsample);
    // debugParams();
  }

  timeref_t t0 = 0; // will be overwritten
  for (auto const &group: groupedchannels) {
    for (uint16 c: group) {
      float const *samplePtr = buffer.getReadPointer(c);
      CyclBuf<raw_t> &inbuf(*inbufs[c]);
      t0 = inbuf.latest();
      for (int n=0; n<nsamples; n++)
        inbuf[t0+n] = samplePtr[n];
      inbuf.donewriting(nsamples);
    }
  }

  timeref_t tlim = (*inbufs.begin()).second->latest();
  if (tlim < delay)
    tlim = 0;
  else
    tlim -= delay;
  
  if (eventchannel >= 0) {
    while (!forcestarts.empty()) {
      timeref_t tfrom = forcestarts.front();
      timeref_t tto = forceends.front();
      //printf("forcepeg %li - %li [%li - %li]\n",
      //     tfrom, tto, t0, tlim);
      if (tto <= tlim) {
        for (auto const &group: groupedchannels) {
          std::packaged_task<void()> task([group, tfrom, tto, this]() {
            for (int c: group)
              fitters[c]->forcepeg(tfrom, tto);
          });
          threadpool.post(task);
        }
        threadpool.wait();
        forcestarts.pop();
        forceends.pop();
      } else if (tfrom < tlim) {
        for (auto const &group: groupedchannels) {
          std::packaged_task<void()> task([group, tfrom, tlim, this]() {
            for (int c: group)
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

  for (auto const &group: groupedchannels) {
    std::packaged_task<void()> task([group, tlim, this]() {
      for (int c: group) 
        fitters[c]->process(tlim);
    });
    threadpool.post(task);
  }
  threadpool.wait();

  for (auto const &group: groupedchannels) {
    for (int c: group) {
      float *samplePtr = buffer.getWritePointer(c, 0);
      CyclBuf<raw_t> &outbuf(*outbufs[c]);
      for (int n=0; n<nsamples; n++)
        samplePtr[n] = outbuf[t0 - delay + n];
    }
  }

  while (!futureEvents.empty()) {
    auto evt = futureEvents.front();
    if (evt.time < t0 + nsamples) {
      result.push_back(OutputEvent{outputEventChannel, evt.time,
                                   eventchannel, evt.state,
                                   evt.time - startsample});
      futureEvents.pop();
    } else {
      break;
    }
  }
  return result;
}

void SalpaModule::train(AudioBuffer<float> &buffer, uint32 nsamples) {
  printf("SALPA: train\n");
  for (auto const &group: groupedchannels) {
    for (uint16 c: group) {
      noise[c].train(buffer.getReadPointer(c), nsamples);
      if (noise[c].chunks() >= TRAINCHUNKS) {
        if (!noiseready)
          printf("SALPA: done training\n");
        noise[c].makeready();
        if (!useabsthr || absthr==0)
          fitters[c]->setthreshold(relthr * noise[c].std());
        if (!noiseready)
          fitters[c]->report();
        noiseready = true; // this ought to be automatically true for all
      } else {
        //      printf("SALPA: more training required (%i)\n", noise.chunks());
      }
    }
  }
}

void SalpaModule::forgetTraining() {
  for (auto &n: noise)
    n.second.reset();
  noiseready = false;
}

void SalpaModule::dropFitters() {
  for (auto &f: fitters)
    delete f.second;
  for (auto &b: inbufs)
    delete b.second;
  for (auto &b: outbufs)
    delete b.second;
  fitters.clear();
  inbufs.clear();
  outbufs.clear();
}

void SalpaModule::updateSettings(uint16 streamId1,
                       juce::OwnedArray<ContinuousChannel> const &channels) {
  streamId = streamId1;
  forgetTraining();
  dropFitters();
  rebuildGroupedChannels(channels);
}

void SalpaModule::rebuildGroupedChannels(juce::OwnedArray<ContinuousChannel>
                                         const &channels) {
  int nChannels = 0;
  for (auto const *channel: channels)
    if (channel->getStreamId()==streamId)
      nChannels++;

  int step = nChannels / NTHREADS;
  if (step*NTHREADS < nChannels)
    step++;

  groupedchannels.clear();
  std::vector<int> group;
  for (auto const *channel: channels) {
    if (channel->getStreamId()==streamId) {
      group.push_back(channel->getGlobalIndex());
      if (group.size() >= step) {
        groupedchannels.push_back(group);
        group.clear();
      }
    }
  }
  if (!group.empty())
    groupedchannels.push_back(group);
}


void SalpaModule::dropFuture() {
  while (!forcestarts.empty())
    forcestarts.pop(); // for some reason, there is no std::queue::clear
  while (!forceends.empty())
    forceends.pop();
  while (!futureEvents.empty())
    futureEvents.pop();
}

void SalpaModule::handleEvent(int channel, bool state, int64 time) {
  if (state && channel==eventchannel) {
    forcestarts.push(time);
    forceends.push(time + t_potblank);
    if (outputEventChannel) {
      futureEvents.push(EventPrep{time + delay, true});
      futureEvents.push(EventPrep{time + t_potblank + delay, false});
    }
  }
}

void SalpaModule::startAcquisition() {
}

