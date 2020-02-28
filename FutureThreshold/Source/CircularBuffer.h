// CircularBuffer.h

#ifndef CIRCULARBUFFER_H

#define CIRCULARBUFFER_H

#include <vector>

class CircularBuffer {
public:
  CircularBuffer(int logsize=12);
  void reset(); // sets delay to zero
  void unget(int n);
  inline void put(float x) {
    data[writeindex & mask] = x;
    writeindex++;
  }
  inline float get() {
    float x = data[readindex & mask];
    readindex++;
    return x;
  }
private:
  std::vector<float> vec;
  int logsize;
  float *data;
  int readindex;
  int writeindex;
  int mask;
};

class CircularBuffers {
public:
  CircularBuffers(int nChannels=512, int logsize=12);
  void reset();
  void unget(int n);
  inline CircularBuffer &channel(int c) { return buffers[c]; }
private:
  std::vector<CircularBuffer> buffers;
  int nchans;
  int logsize;
};

#endif
