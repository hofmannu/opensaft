#include "Util/Logger.h"
#include <chrono>
#include <cxxabi.h>
#include <iostream>

namespace opensaft {

std::string demangle(const char* name) {

  int status = -4; // some arbitrary value to eliminate the compiler warning

  // enable c++11 by passing the flag -std=c++11 to g++
  std::unique_ptr<char, void (*)(void*)> res{
      abi::__cxa_demangle(name, NULL, NULL, &status), std::free};

  return (status == 0) ? res.get() : name;
}

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

void Logger::Log(LogHeader header, const std::string& message) {
  // log message to console
  std::cout << GetTimestamp() << " ("
            << LogLevelNames[static_cast<std::size_t>(header.level)] << ")";
  if (!header.origin.empty())
    std::cout << " [" << header.origin << "] ";
  std::cout << message << std::endl;
}

void Logger::Log(const std::string& message) { Log(LogLevel::Info, message); }

std::string LoggingClass::GetName() const {
  return demangle(typeid(*this).name());
}

} // namespace opensaft