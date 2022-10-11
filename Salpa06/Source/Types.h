/* common/Types.H: part of meabench, an MEA recording and analysis tool
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

// Types.H

#ifndef TYPES_H

#define TYPES_H

// // Magic incantation to get 64-bit integers on most platforms
// // Inspired by mathworks's "io64.h" file.
// #if defined(__alpha) || defined(__sparcv9) || defined(__ia64) \
//     || defined(__ia64__) || defined(__x86_64__) || defined(__LP64__)
//   #define UINT64_DW unsigned long
// #elif defined(_MSC_VER) || (defined(__BORLANDC__) && __BORLANDC__ >= 0x530) \
//       || (defined(__WATCOMC__)  && __WATCOMC__  >= 1100)
//   #define UINT64_DW unsigned __int64
// #elif defined(__GNUC__) || defined(__hpux) || defined(__sun)
//   #define UINT64_DW unsigned long long
// #else
//   #define UINT64_DW unsigned long long
// #endif
// 
// typedef UINT64_DW timeref_t;

#include <cstdint>
#include <sstream>

typedef std::uint64_t timeref_t;
constexpr timeref_t INFTY{~timeref_t(0)};

typedef std::int64_t int_t;
typedef std::uint64_t uint_t;

typedef float raw_t; //:t raw_t
/*:D Type of raw data, i.e. single channel samples. */

typedef double real_t; //:t real_t
/*:D Representation for real numbers used throughout this software. */

typedef std::uint8_t byte;

#include <iostream>

class Dbg {
public:
  Dbg() {
  }
  ~Dbg() {
    std::cerr << str.str() << "\n";
    std::cerr.flush();
  }
  template <class T> Dbg &operator<<(T const &t) { str << t; return *this; }
private:
  std::ostringstream str;
};    

class Error {
public:
  Error(char const *issuer0=0, char const *cause0=0):
    issuer(issuer0), cause(cause0) {
  }
  virtual ~Error() {}
  void report(char const *src=0) const {
    Dbg dbg;
    if (src)
      dbg << src << ": ";
    dbg << issuer << ": ";
    dbg << cause;
  }
protected:
  std::string issuer;
  std::string cause;
};


#endif
