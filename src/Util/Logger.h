
#include <array>
#include <string>

#pragma once

namespace opensaft {

enum class LogLevel : int {
  Debug = 0, //!< Debug is the lowest level
  Info,
  Warning,
  Error,
  Critical,
  _Count //!< number of log types
};

constexpr std::array<std::string, static_cast<std::size_t>(LogLevel::_Count)>
    LogLevelNames{"Debug", "Info", "Warning", "Error", "Critical"};

class Logger {
public:
  static void Log(LogLevel level, const std::string& message);

  /// will default to LogLevel::Info
  static void Log(const std::string& message);

private:
  static std::string GetTimestamp();
};
} // namespace opensaft