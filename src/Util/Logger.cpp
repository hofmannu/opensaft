#include "Util/Logger.h"
#include <chrono>
#include <iostream>

namespace opensaft {

std::string Logger::GetTimestamp() {
  // get current time
  auto now = std::chrono::system_clock::now();
  // convert to time_t
  auto now_c = std::chrono::system_clock::to_time_t(now);
  // convert to string
  std::stringstream ss;
  ss << std::put_time(std::localtime(&now_c), "%F %T");
  return ss.str();
}

void Logger::Log(LogLevel level, const std::string& message) {
  // log message to console
  std::cout << GetTimestamp() << " ["
            << LogLevelNames[static_cast<std::size_t>(level)] << "]" << message
            << std::endl;
}

void Logger::Log(const std::string& message) { Log(LogLevel::Info, message); }

} // namespace opensaft