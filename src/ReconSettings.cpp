#include "ReconSettings.h"

namespace opensaft {

void ReconSettings::set_sos(const float _sos) { sos = _sos; }

void ReconSettings::set_flagUs(const bool _flagUs) { flagUs = _flagUs; }

void ReconSettings::set_cropTMin(const float tMin) { cropMm[0] = tMin; }
void ReconSettings::set_cropTMax(const float tMax) { cropMm[1] = tMax; }
void ReconSettings::set_cropXMin(const float xMin) { cropMm[2] = xMin; }
void ReconSettings::set_cropXMax(const float xMax) { cropMm[3] = xMax; }
void ReconSettings::set_cropYMin(const float yMin) { cropMm[4] = yMin; }
void ReconSettings::set_cropYMax(const float yMax) { cropMm[5] = yMax; }

void ReconSettings::set_cropT(const float tMin, const float tMax) {
  cropMm[0] = tMin;
  cropMm[1] = tMax;
}

void ReconSettings::set_cropX(const float xMin, const float xMax) {
  cropMm[2] = xMin;
  cropMm[3] = xMax;
}

void ReconSettings::set_cropY(const float yMin, const float yMax) {
  cropMm[4] = yMin;
  cropMm[5] = yMax;
}

void ReconSettings::set_crop(const uint8_t iDim, const float minVal,
                             const float maxVal) {
  cropMm[iDim * 2] = minVal;
  cropMm[iDim * 2 + 1] = maxVal;
}

// checks for each dimension if the larger cropping value is bigger then the
// smaller if not it swaps them
void ReconSettings::sortCropping() {
  for (uint8_t iDim = 0; iDim < 3; iDim++) {
    if (cropMm[iDim * 2] > cropMm[iDim * 2 + 1]) {
      const float backup = cropMm[iDim * 2];
      cropMm[iDim * 2] = cropMm[iDim * 2 + 1];
      cropMm[iDim * 2 + 1] = backup;
    }
  }
}

void ReconSettings::set_rMin(const float _rMin) { rMin = _rMin; }

} // namespace opensaft