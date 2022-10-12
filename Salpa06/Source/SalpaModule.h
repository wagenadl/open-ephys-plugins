// SalpaModule.h

#ifndef SALPASETTINGS_H

#define SALPASETTINGS_H

#include "Types.h"
#include "LocalFit.h"
#include "CyclBuf.h"
#include "NoiseLevel.h"
#include "TaskQueue.h"
#include <map>
#include <ProcessorHeaders.h>

constexpr int FREQKHZ = 30; // whatever
constexpr int TRAINCHUNKS = 20;
constexpr int LOGBUFSIZE = 12; // BUFSIZE = 1<<12 = 4096 samples
constexpr int NTHREADS = 8; // or 16

class SalpaModule {
public:
  struct OutputEvent {
    EventChannel *channelptr;
    int64 time;
    int line;
    int state;
    int64 reltime;
  };
public:
  SalpaModule();
  ~SalpaModule();
public:
  void createFitters(int64 startsample);
  void dropFitters();
  void forgetTraining();
  void updateSettings(uint16 streamId,
                      juce::OwnedArray<ContinuousChannel> const &channels);
  void startAcquisition();
  std::list<OutputEvent> process(AudioBuffer<float> &buffer,
                                 int64 startsample,
                                 uint32 nsamples);
  void handleEvent(int channel, bool state, int64 time);
  
public:
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
private:
  void rebuildGroupedChannels(juce::OwnedArray<ContinuousChannel>
                              const &channels);
  void dropFuture();
  void train(AudioBuffer<float> &buffer,
             uint32 nsamples);
public:
  EventChannel *outputEventChannel;
private:
  uint16 streamId;
  std::vector<std::vector<int>> groupedchannels;
  // helpers
  std::map<uint16, LocalFit *> fitters;
  std::map<uint16, CyclBuf<raw_t> *> inbufs;
  std::map<uint16, CyclBuf<raw_t> *> outbufs;
  std::map<uint16, NoiseLevel> noise;
  bool noiseready;
  int delay;
  std::queue<timeref_t> forcestarts;
  std::queue<timeref_t> forceends;
  timeref_t t0;
  struct EventPrep {
    timeref_t time;
    int state;
  };
  std::queue<EventPrep> futureEvents;
  TaskQueue<std::packaged_task<void()>> threadpool;

};

#endif
