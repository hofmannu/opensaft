#include "Saft.h"
#include "Util/Logger.h"
#include "Util/Timer.h"
#include <format>

namespace opensaft {

Saft::Saft() : m_processorCount(std::thread::hardware_concurrency()) {
  Log(std::format("Found {} processor units...", m_processorCount));
}

// for each a scan first calculate the mean and then substract if from the
// vector
void Saft::RemoveDC() {

  Log("Removing DC offset... ");
  Timer T;
  for (auto& sigs : *m_measuredData)
    sigs.RemoveDC();

  T.Stop();
  Log(std::format("DC offset removed in {} seconds", T.GetElapsedTime()));
}

void Saft::Launch() {
  // if input data was not specified, we cannot do anything
  if (!m_measuredData.has_value()) {
    Log(LogLevel::Warning, "No data available for reconstruction!");
    return;
  }

  m_start = std::chrono::high_resolution_clock::now();
  m_isRunning = true;
  Log("Starting reconstruction...");
  m_reconThread = std::thread(&Saft::Recon, this);
}

void Saft::Recon() {

  m_percDone = 0.0f;

  // remove the DC component
  RemoveDC();

  // for now we just sleep a bit, then finish
  for (std::size_t iRound = 0; iRound < 100; iRound++) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    m_percDone = static_cast<float>(iRound);
    // todo here we need to insert some sort of calculation
  }

  // indicate that we are finished and calculate reconstruction time
  m_isRunning = false;
  m_percDone = 100.0f;
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - m_start;
  m_reconTime = elapsed.count();
}

void Saft::Wait() {
  while (m_isRunning) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    Log(std::format("Reconstruction status at {}%", get_percDone()));
  }

  if (m_reconThread.joinable()) {
    m_reconThread.join();
  }

  Log(std::format("Reconstruction finished after {} seconds", m_reconTime));
}

std::optional<Volume> Saft::GetVolume() const {
  if (!m_reconData.has_value()) {
    Log(LogLevel::Warning, "No volume data available");
  }
  return m_reconData;
}

float Saft::CalculateVoxel(Size3 idx) {
  // calculates the response for a single position in the volume
  // todo not implemented yet
  //
  Float3 posOutput = m_reconData.value().get_pos(idx);

  return 0.0f;
}

} // namespace opensaft