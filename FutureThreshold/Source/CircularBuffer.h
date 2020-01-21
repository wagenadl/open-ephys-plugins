// CircularBuffer.h

#ifndef CIRCULARBUFFER_H

#define CIRCULARBUFFER_H

class CircularBuffer {
public:
  CircularBuffer(int logsize=12);
  ~CircularBuffer();
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
  int logsize;
  float *data;
  int readindex;
  int writeindex;
  int mask;
};

class CircularBuffers {
public:
  CircularBuffers(int nChannels=512, int logsize=12);
  ~CircularBuffers();
  void reset();
  void unget(int n);
  inline CircularBuffer *channel(int c) { return buffers[c]; }
private:
  CircularBuffer **buffers;
  int nchans;
  int logsize;
};

#endif
