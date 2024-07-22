#include "Memory/UltrasoundSignals.h"
#include "Memory/Volume.h"
#include "ReconSettings.h"
#include "Transducer.h"
#include "Util/Logger.h"
#include <optional>
#include <thread>

#pragma once

namespace opensaft {

struct dcData {
  uint64_t iVecStart;
  uint64_t iVecEnd;
  uint64_t nT;
  uint64_t threadId;
  float* ptr;
};

class Saft : LoggingClass {

public:
  // class constructor
  Saft();

  /// runs the actual reconstruction in a separate thread
  void Launch();

  /// waiting for the reconstruction to finish
  void Wait();

  /// define the input data for the reconstruction
  void SetInput(UltrasoundSignals&& us) { m_measuredData = std::move(us); }

  std::optional<Volume> GetVolume() const;

  void saft_cpu(); // cpu version of the kernel, used for debugging and if no
                   // GPU present

  /// substracts the DC component from the signal so that mean(sig) is close to
  /// 0 for each individual waveform
  void RemoveDC();

  [[nodiscard]] double get_reconTime() const noexcept { return m_reconTime; };
  [[nodiscard]] double get_tRemain() const noexcept { return tRemain; };

  [[nodiscard]] bool get_isRunning() const noexcept { return m_isRunning; };
  [[nodiscard]] float get_percDone() const noexcept { return m_percDone; };

private:
  void Recon();

  /// calculates the response of a single voxel in the output volume
  float CalculateVoxel(Size3 idx);

  std::optional<UltrasoundSignals> m_measuredData; //!< preprocessed datasets
  std::optional<Volume> m_reconData;               //!< reconstructed datasets
  std::thread m_reconThread; //!< thread for reconstruction

  std::chrono::time_point<std::chrono::high_resolution_clock>
      m_start; //!< time of start

  const unsigned int m_processorCount;   //!< total number of cores available
  std::atomic<bool> m_isRunning = false; //!< is reconstruction running
  std::atomic<float> m_percDone =
      0.0f;                 //!< perc of reconstruction done so far [%]
  double tRemain = 0;       //!< remaining reconstruction time (estimate)
  double m_reconTime = 0.0; //!< time of last reconstruction
};

} // namespace opensaft