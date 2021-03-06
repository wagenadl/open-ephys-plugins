/* base/CyclBuf.H: part of meabench, an MEA recording and analysis tool
** Copyright (C) 2000-2002  Daniel Wagenaar (wagenaar@caltech.edu)
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

// CyclBuf.H

#ifndef CYCLBUF_H

#define CYCLBUF_H

#include "Types.h"
#include <vector>
#include <algorithm>

template <class T> class CyclBuf {
public:
  CyclBuf(uint_t logsiz0): logsiz(logsiz0) {
    timeref_t siz = 1; siz <<= logsiz;
    vec.resize(siz);
    data_ = vec.data();
    latest_ = 0;
    mask = siz - 1;
  }
  void fill(T t) { std::fill(vec.begin(), vec.end(), t); }
  timeref_t latest() const { return latest_; }
  T const &operator[](timeref_t t) const { return data_[t&mask]; }
  T &operator[](timeref_t t) { return data_[t&mask]; }
  T *wheretowrite() { return data_ + (latest_&mask); }
  void donewriting(int_t n) { latest_ += n; }
  void reset(timeref_t t=0) { latest_=t; }
private:
  std::vector<T> vec;
  T *data_;
  timeref_t latest_;
  timeref_t mask;
  uint_t logsiz;
};

#endif
