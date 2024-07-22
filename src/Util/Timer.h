
#include <chrono>
#include <optional>

#pragma once

namespace opensaft {

/// a simple class that allows timing, estimating remaining progress etc
class Timer {
public:
  Timer(bool autoStart = true);

  /// starts the timer, throws if already started
  void Start();

  /// stops the timer, throws if already stopped
  void Stop();

  /// returns the elapsed time in seconds
  [[nodiscard]] double GetElapsedTime() const;

private:
  std::optional<std::chrono::time_point<std::chrono::high_resolution_clock>>
      m_start;
  std::optional<std::chrono::time_point<std::chrono::high_resolution_clock>>
      m_stop;
};

} // namespace opensaft