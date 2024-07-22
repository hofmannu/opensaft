

#include "Util/Timer.h"
#include <stdexcept>

namespace opensaft {

Timer::Timer(const bool autoStart) {
  if (autoStart) {
    Start();
  }
}

void Timer::Start() {
  if (m_start.has_value()) {
    throw std::runtime_error("Timer already started");
  }
  m_start = std::chrono::high_resolution_clock::now();
}

void Timer::Stop() {
  if (!m_start.has_value()) {
    throw std::runtime_error("Timer not started");
  }

  if (m_stop.has_value()) {
    throw std::runtime_error("Timer already stopped");
  }

  m_stop = std::chrono::high_resolution_clock::now();
}

double Timer::GetElapsedTime() const {
  if (!m_start.has_value()) {
    throw std::runtime_error("Timer not started");
  }

  if (!m_stop.has_value()) {
    throw std::runtime_error("Timer not stopped");
  }

  return std::chrono::duration<double>(m_stop.value() - m_start.value())
      .count();
}

} // namespace opensaft