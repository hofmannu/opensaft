#include "transducer.h"

#include <stdexcept>

void transducer::set_focalDistance(const float _focalDistance) {
  if (_focalDistance < 0.0f)
    throw std::invalid_argument("focal distance must be positive");

  focalDistance = _focalDistance;
}

void transducer::set_rAperture(const float _rAperture) {
  if (_rAperture < 0.0f)
    throw std::invalid_argument("aperture radius must be positive");

  rAperture = _rAperture;
}

void transducer::set_rHole(const float _rHole) {
  if (_rHole < 0.0f)
    throw std::invalid_argument("hole radius must be positive");

  rHole = _rHole;
}