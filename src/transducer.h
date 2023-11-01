/*
        class representing a spherically focused ultrasound transducer
        Author: Urs Hofmann
        Mail: hofmannu@ethz.ch
        Date: 27.12.2020
*/

#include <math.h>

#pragma once
class transducer {
 public:
  /// \brief returns
  [[nodiscard]] float* get_pfocalDistance() { return &focalDistance; };
  [[nodiscard]] float* get_prAperture() { return &rAperture; };
  [[nodiscard]] float* get_rHole() { return &rHole; };

  void set_focalDistance(const float _focalDistance);
  void set_rAperture(const float _rAperture);
  void set_rHole(const float _rHole);

  // return actual values for transducer properties
  [[nodiscard]] float get_focalDistance() const noexcept {
    return focalDistance;
  };
  [[nodiscard]] float get_rAperture() const noexcept { return rAperture; };
  [[nodiscard]] float get_rHole() const noexcept { return rHole; };
  [[nodiscard]] float get_theta() const noexcept {
    return asin(rAperture / focalDistance);
  };

 private:
  float focalDistance = 7.0f;  // focal distance of transducer [mm]
  float rAperture = 3.2f;      // radius of aperture of transducer [mm]
  float rHole = 0.5f;          // radius of central hole of transducer [mm]
};
