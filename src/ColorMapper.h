/*
  c++ tooolbox for simple mapping from numbers to colors
  Author: Urs Hofmann
  Mail: mail@hofmannu.org
  Date: 25.09.2020
*/

#pragma once

#include "VectorN.h"
#include <cstdint>
#include <cstdio>

class ColorMapper {
private:
  float minVal = -1.0f; //!< minimum value in colormap
  float maxVal = 1.0f;  //!< maximum value in colormap
  float maxAbsVal = 1.0f;
  float span = 2.0f;
  Float4 minCol{0.0f}; //!< color assigned to min
  Float4 maxCol{1.0f}; //!< color assigned to max
  Float4 spanCol{1.0f};

  void calc_span_col();
  void calc_max_abs();

  uint8_t mapType = 0; // todo make this an enum class
                       // 0: linear rgb
                       // 1: diverging linear rgb (crossing 0 at 0)
                       //
public:
  // set function
  void set_maxVal(const float _maxVal);
  void set_minVal(const float _minVal);
  void set_minCol(const Float4 &_minCol); // 4 element vector passed
  void set_maxCol(const Float4 &_maxVal); // 4 element vector passed
  void set_mapType(const uint8_t _mapType);

  // get pointer to min and max value
  [[nodiscard]] float *get_pmaxVal() { return &maxVal; };
  [[nodiscard]] float *get_pminVal() { return &minVal; };
  [[nodiscard]] float get_maxVal() const noexcept { return maxVal; };
  [[nodiscard]] float get_minVal() const noexcept { return minVal; };
  [[nodiscard]] float *get_pminCol() { return &minCol[0]; };
  [[nodiscard]] float *get_pmaxCol() { return &maxCol[0]; };

  // transfer function
  void convert_to_map(const float *dataIn, const uint64_t nElem,
                      unsigned char *dataOut) const;

  void convert_to_rgba(const float *dataIn, const uint64_t nElem,
                       unsigned char *dataOut) const;

  void convert_to_divmap(const float *dataIn, const uint64_t nElem,
                         unsigned char *dataOut) const;
};
