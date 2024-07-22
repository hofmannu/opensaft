
#include "VectorN.h"
#include <vector>

#pragma once

namespace opensaft {
struct TimeSignalProperties {
  float sampleRate{1e9f}; //!< sampling rate of the acquisition [Hz]
  float deltaT{0.0f};     //!< time between laser emission & first sample [sec]
  Float3 pos{0.0f};       //!< transducer position in global coordinate system
};

class TimeSignal : public std::vector<float>, TimeSignalProperties {
public:
  TimeSignal() = default;
  TimeSignal(const std::size_t nt) : std::vector<float>(nt, 0.0f) {}

  /// removes the DC content of a signal so that its mean is zero
  void RemoveDC();

private:
};

struct AcquisitionProperties {};

class UltrasoundSignals : public std::vector<TimeSignal>,
                          AcquisitionProperties {

public:
private:
};

} // namespace opensaft