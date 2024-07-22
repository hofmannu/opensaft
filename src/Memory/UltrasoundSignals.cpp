#include "Memory/UltrasoundSignals.h"

namespace opensaft {

void TimeSignal::RemoveDC() {
  // calculate mean
  double mean = 0.0;
  for (size_t i = 0; i < this->size(); i++) {
    mean += this->at(i);
  }
  mean /= this->size();

  // remove mean
  for (size_t i = 0; i < this->size(); i++) {
    this->at(i) -= mean;
  }
}

} // namespace opensaft