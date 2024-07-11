#include "ColorMapper.h"
#include "MathUtil.h"
#include <math.h>

void ColorMapper::set_maxVal(const float _maxVal) {
  maxVal = _maxVal;
  calc_max_abs();
}

void ColorMapper::set_minVal(const float _minVal) {
  minVal = _minVal;
  calc_max_abs();
}

void ColorMapper::set_minCol(const Float4 &_minCol) {
  minCol = _minCol;
  calc_span_col();
}

void ColorMapper::set_maxCol(const Float4 &_maxCol) {
  maxCol = _maxCol;
  calc_span_col();
}

void ColorMapper::calc_span_col() { spanCol = maxCol - minCol; }

void ColorMapper::calc_max_abs() {
  maxAbsVal = (fabsf(maxVal) > fabsf(minVal)) ? fabsf(maxVal) : fabsf(minVal);
}

void ColorMapper::convert_to_rgba(const float *dataIn, const uint64_t nElem,
                                  unsigned char *dataOut) const {
  float spanTemp = maxVal - minVal;
  Float4 spanColTemp = maxCol - minCol;

  // scale whole array to range from to 0 to 1
  float temp;
  for (uint64_t iElem = 0; iElem < nElem; iElem++) {
    temp = (dataIn[iElem] - minVal) / spanTemp; // scale to [0 ... 1]
    temp = Clamp(temp, 0.0f, 1.0f);

    for (unsigned char iCol = 0; iCol < 4; iCol++)
      dataOut[iCol + iElem * 4] =
          (minCol[iCol] + temp * spanColTemp[iCol]) * 255;
  }
}

// make diverging colormap centered around 0 or 1
void ColorMapper::convert_to_divmap(const float *dataIn, const uint64_t nElem,
                                    unsigned char *dataOut) const {
  // scale whole array to range from to 0 to 1
  float temp;
  for (uint64_t iElem = 0; iElem < nElem; iElem++) {
    temp = abs(dataIn[iElem]) / maxAbsVal; // scale to [0 ... 1]
    if (temp > 1.0f)
      temp = 1.0f;

    if (dataIn[iElem] < 0) {
#pragma unroll
      for (unsigned char iCol = 0; iCol < 4; iCol++) {
        dataOut[iCol + iElem * 4] = (1 - temp * minCol[iCol]) * 255;
      }

    } else {
#pragma unroll
      for (unsigned char iCol = 0; iCol < 4; iCol++)
        dataOut[iCol + iElem * 4] = (1 - temp * maxCol[iCol]) * 255;
    }
    dataOut[4] = 255;
  }
}

void ColorMapper::convert_to_map(const float *dataIn, const uint64_t nElem,
                                 unsigned char *dataOut) const {
  if (mapType == 0)
    convert_to_rgba(dataIn, nElem, dataOut);
  else if (mapType == 1) {
    convert_to_divmap(dataIn, nElem, dataOut);
    // set default colors for diverging map
  }
}

void ColorMapper::set_mapType(const uint8_t _mapType) {
  mapType = _mapType;
  if (mapType == 0) {
    minCol = 0.0f;
    maxCol = 1.0f;
  }
  if (mapType == 1) {
    minCol = {1.0f, 1.0f, 0.0f, 1.0f}; // set minVal to bright blue
    maxCol = {0.0f, 1.0f, 1.0f, 1.0f}; // set maxVal to bright red
  }
}