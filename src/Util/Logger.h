
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

struct LogHeader {
  LogLevel level;
  std::string origin = "";

  LogHeader(LogLevel level, const std::string& origin)
      : level(level), origin(origin) {}

  LogHeader(LogLevel level) : level(level) {}
};

constexpr std::array<std::string, static_cast<std::size_t>(LogLevel::_Count)>
    LogLevelNames{"Debug", "Info", "Warning", "Error", "Critical"};

class Logger {
public:
  static void Log(LogHeader header, const std::string& message);

  /// will default to LogLevel::Info
  static void Log(const std::string& message);

private:
  static std::string GetTimestamp();
};

/// this class is used to inherit from and automatically populate the name tag
class LoggingClass {
public:
  void Log(LogLevel level, const std::string& message) const {
    Logger::Log({level, GetName()}, message);
  }

  void Log(const std::string& message) const {
    Logger::Log({LogLevel::Info, GetName()}, message);
  }

private:
  virtual std::string GetName() const;
};
} // namespace opensaft