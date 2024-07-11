#include "Saft.h"
#include <iostream>
#include <thread>

namespace opensaft {

Saft::Saft() {
  processor_count = std::thread::hardware_concurrency();
  printf("[Saft] Found %d processor units... \n", processor_count);
}

// crop the image to the field of view
void Saft::crop() {
  // check if all max values are bigger then the minimum values, otherwise swap
  // them
  sett.sortCropping();

  // find index closest to minimum cropping positions
  uint64_t idxMin[3];
  uint64_t idxMax[3];
  for (uint8_t iDim = 0; iDim < 3; iDim++) {
    idxMin[iDim] = preprocData.get_idx(sett.get_cropMin(iDim) * 1e-3, iDim);
    // printf("min idx along %d is %d \n", iDim, idxMin[iDim]);
    // find index closest to maximum cropping positions
    idxMax[iDim] = preprocData.get_idx(sett.get_cropMax(iDim) * 1e-3, iDim);
    // printf("max idx alomg %d is %d \n", iDim, idxMax[iDim]);
  }

  // push res origin and size over from original dataset
  for (uint8_t iDim = 0; iDim < 3; iDim++) {
    croppedData.set_origin(iDim, // push origin
                           preprocData.get_pos(idxMin[iDim], iDim));

    croppedData.set_res(iDim, // push resolution
                        preprocData.get_res(iDim));
    // push new size
    croppedData.set_dim(iDim, idxMax[iDim] - idxMin[iDim] + 1);
  }
  croppedData.alloc_memory();

  // fill freshly allocated memory
  uint64_t runIdx = 0;
#pragma unroll
  for (uint64_t iy = idxMin[2]; iy < idxMax[2]; iy++) {
    const uint64_t iyRel = iy - idxMin[2];
#pragma unroll
    for (uint64_t ix = idxMin[1]; ix < idxMax[1]; ix++) {
      const uint64_t ixRel = ix - idxMin[1];
#pragma unroll
      for (uint64_t it = idxMin[0]; it < idxMax[0]; it++) {
        const uint64_t itRel = it - idxMin[0];
        croppedData.set_value(itRel, ixRel, iyRel,
                              preprocData.get_value(it, ix, iy));
        runIdx++;
      }
    }
  }

  return;
}

void* RemoveDc(void* threadarg) {
  struct dcData* myData;
  myData = (struct dcData*)threadarg;

#pragma unroll
  for (uint64_t iVec = myData->iVecStart; iVec <= myData->iVecEnd; iVec++) {
    // calculate the full sum over the vector
    float sum = 0;
    uint64_t startIdx = iVec * myData->nT;
    for (uint64_t iT = 0; iT < myData->nT; iT++) {
      sum += myData->ptr[startIdx];
      startIdx++;
    }
    sum = sum / ((float)myData->nT);

    startIdx = iVec * myData->nT;
    for (uint64_t iT = 0; iT < myData->nT; iT++) {
      myData->ptr[startIdx] = myData->ptr[startIdx] - sum;
      startIdx++;
    }
  }
  pthread_exit(NULL);
}

// for each a scan first calculate the mean and then substract if from the
// vector
void Saft::remove_dc() {
  std::cout << "[Saft] Removing DC offset... " << std::flush;

  pthread_t threads[processor_count];
  pthread_attr_t attr;
  void* status;

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  struct dcData td[processor_count];

  uint64_t nVecs = croppedData.get_dim(1) * croppedData.get_dim(2);
  const uint64_t nProcessorVecs = ceil((float)nVecs / ((float)processor_count));

  // start individual threads taking care of dc removal
  for (uint64_t iProcessor = 0; iProcessor < processor_count; iProcessor++) {
    td[iProcessor].threadId = iProcessor;
    td[iProcessor].iVecStart = iProcessor * nProcessorVecs;
    td[iProcessor].iVecEnd = (iProcessor + 1) * nProcessorVecs;
    if (td[iProcessor].iVecEnd >= nVecs) {
      td[iProcessor].iVecEnd = nVecs - 1;
    }
    td[iProcessor].nT = croppedData.get_dim(0);
    td[iProcessor].ptr = croppedData.get_pdata();

    int rc = pthread_create(&threads[iProcessor], &attr, RemoveDc,
                            (void*)&td[iProcessor]);
    if (rc) {
      cout << "Error:unable to create thread," << rc << endl;
      exit(-1);
    }
  }

  // wait for ecxecution
  pthread_attr_destroy(&attr);
  for (uint64_t iProcessor = 0; iProcessor < processor_count; iProcessor++) {
    int rc = pthread_join(threads[iProcessor], &status);
    if (rc) {
      cout << "Error: unable to join," << rc << endl;
      exit(-1);
    }
  }

  std::cout << "done!" << std::endl;
}

} // namespace opensaft