// CircularBuffer.cpp

#include "CircularBuffer.h"

CircularBuffer::CircularBuffer(int logsize):
  logsize(logsize) {
  data = new float[1<<logsize];
  mask = (1<<logsize) - 1;
  reset();
}

void CircularBuffer::reset() {
  readindex = writeindex = 0;
  for (int k=0; k<(1<<logsize); k++)
    data[k] = 0;
}

CircularBuffer::~CircularBuffer() {
  delete [] data;
}

void CircularBuffer::unget(int n) {
  for (int k=0; k<n; k++) {
    readindex --;
    data[readindex & mask] = 0;
  }
}

CircularBuffers::CircularBuffers(int nchans, int logsize): nchans(nchans), logsize(logsize) {
  buffers = new CircularBuffer *[nchans];
  for (int c=0; c<nchans; c++)
    buffers[c] = new CircularBuffer(logsize);
}

CircularBuffers::~CircularBuffers() {
  for (int c=0; c<nchans; c++)
    delete buffers[c];
  delete [] buffers;
}

void CircularBuffers::reset() {
  for (int c=0; c<nchans; c++)
    buffers[c]->reset();
}

void CircularBuffers::unget(int n) {
  for (int c=0; c<nchans; c++)
    buffers[c]->unget(n);
}

  
