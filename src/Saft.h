
#pragma once

#include "ReconSettings.h"
#include "Transducer.h"
#include "volume.h"
#include <chrono> // used to stop time which is required for execution
#include <cstdio>
#include <pthread.h>
#include <thread>

using namespace std;

namespace opensaft {

struct dcData {
  uint64_t iVecStart;
  uint64_t iVecEnd;
  uint64_t nT;
  uint64_t threadId;
  float* ptr;
};

class Saft {

public:
  // class constructor
  Saft();

  Transducer* get_ptrans() { return &trans; };
  ReconSettings* get_psett() { return &sett; };
  volume* get_ppreprocData() { return &preprocData; };
  volume* get_preconData() { return &reconData; };
  volume* get_pcroppedData() { return &croppedData; };

  void recon();
  std::thread recon2thread();

  void saft_cpu(); // cpu version of the kernel, used for debugging and if no
                   // GPU present
  void crop();
  void remove_dc();

  [[nodiscard]] double get_reconTime() const noexcept { return reconTime; };
  [[nodiscard]] double get_tRemain() const noexcept { return tRemain; };
  std::chrono::time_point<std::chrono::system_clock> get_tStart() const {
    return tStart;
  };

  [[nodiscard]] bool get_isRunning() const noexcept { return isRunning; };
  [[nodiscard]] float get_percDone() const noexcept { return percDone; };

private:
  Transducer trans;   // settings of the used transducer
  ReconSettings sett; // reconstruction settings
  volume preprocData; // preprocessed datasets
  volume reconData;   // reconstructed datasets
  volume croppedData; // cropped dataset

  int processor_count = 1;
  bool isRunning =
      false; // flag indicating if reconstruction is currently running
  float percDone = 0.0f; // perc of reconstruction done so far [%]

  // all we need to time the execution
  std::chrono::time_point<std::chrono::system_clock> tStart; // start time
  std::chrono::time_point<std::chrono::system_clock> tEnd;   // end time
  double tRemain = 0;
  double reconTime = 0; // time required for last reconstruction
};

} // namespace opensaft