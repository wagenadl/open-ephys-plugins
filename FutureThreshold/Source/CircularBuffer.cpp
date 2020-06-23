// CircularBuffer.cpp

#include "CircularBuffer.h"
#include <algorithm>

CircularBuffer::CircularBuffer(int logsize):
  logsize(logsize) {
  int size = 1<<logsize;
  vec.resize(size);
  data = vec.data();
  mask = size - 1;
  reset();
}

void CircularBuffer::reset() {
  readindex = writeindex = 0;
  std::fill(vec.begin(), vec.end(), 0);
}

void CircularBuffer::unget(int n) {
  for (int k=0; k<n; k++) {
    readindex --;
    data[readindex & mask] = 0;
  }
}

CircularBuffers::CircularBuffers(int nchans, int logsize): nchans(nchans), logsize(logsize) {
  for (int c=0; c<nchans; c++)
    buffers.push_back(CircularBuffer(logsize));
}

void CircularBuffers::reset() {
  for (int c=0; c<nchans; c++)
    buffers[c].reset();
}

void CircularBuffers::unget(int n) {
  for (int c=0; c<nchans; c++)
    buffers[c].unget(n);
}

  
