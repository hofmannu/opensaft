#include "saft.cuh"

// gpu kernel for SAFT
__global__ void
SAFT(float* outputVol,        // reconstructed output volume [it, ix, iy]
     const float* inputVol,   // input signal matrix [it, ix, iy]
     const int nT,            // number of elemetns in time
     const int nX,            // number of elements along x
     const int nY,            // number of elements along y
     const float dt,          // resolution in time domain
     const float dx,          // resolution along x [m]
     const float dy,          // resolution along y [m]
     const float t0,          // time of first index
     const float c0,          // speed of sound in medium [m/s]
     const float critRatio,   // critical ratio betwen rDist and deltaZ
     const float fd,          // focal distance
     const float rMin,        // this is the minimum size accepted for any angle
     const bool flagCfWeight, // should we do coherence factor weighting?
     volatile int* progressCounter) {
  // get index in output volume
  const int zIm = blockIdx.x * blockDim.x + threadIdx.x;
  const int xIm = blockIdx.y * blockDim.y + threadIdx.y;
  const int yIm = blockIdx.z * blockDim.z + threadIdx.z;

  if ((xIm < nX) && (yIm < nY) && (zIm < nT)) {
    const float zDepth =
        (t0 + dt * (float)zIm) * c0; // depth of reconstructed point

    // 1 if far field, -1 if close field
    const int signMultip = -1 + 2 * ((float)(zDepth >= fd));

    const float deltaZ =
        zDepth - fd;     // z distance between focal point and reson point
    float rfsaft = 0;    // coherent saft sum
    float rfsaftabs = 0; // incoherent saft sum
    const int idxFoc = (fd / c0 - t0) / dt + 0.5; // t idx of focal point

    // limit reconstruction range to a local subfield
    const int yConsider =
        critRatio * fabsf(deltaZ) / dy; // index range to consider in y
    const int xConsider =
        critRatio * fabsf(deltaZ) / dx; // index range to consider in x
    const int yStartIdx = ((yIm - yConsider) <= 0) ? 0 : yIm - yConsider;
    const int xStartIdx = ((xIm - xConsider) <= 0) ? 0 : xIm - xConsider;
    const int yStopIdx = ((yIm + yConsider) >= nY) ? nY - 1 : yIm + yConsider;
    const int xStopIdx = ((xIm + xConsider) >= nX) ? nX - 1 : xIm + xConsider;

    int nElem = 0;

#pragma unroll
    for (int iY = yStartIdx; iY <= yStopIdx;
         iY++) // run over all other a scans in y
    {
      const float yRel = dy * ((float)(iY - yIm));
#pragma unroll
      for (int iX = xStartIdx; iX <= xStopIdx;
           iX++) // run over all other a scans in x
      {
        const float xRel = dx * ((float)(iX - xIm));
        const float rDist = sqrtf(xRel * xRel + yRel * yRel);
        const float currRatio = rDist / fabsf(deltaZ);

        if ((currRatio <= critRatio) || (rDist < rMin)) {
          // calculate distance, delay time, and index
          const float distTot = sqrtf(rDist * rDist + deltaZ * deltaZ);
          const int deltaT = distTot / c0 / dt + 0.5;
          const int tIdx = idxFoc + deltaT * signMultip;

          if ((tIdx >= 0) && (tIdx < nT)) {
            const int dataIdx = tIdx + nT * (iX + nX * iY);
            rfsaft += inputVol[dataIdx];
            rfsaftabs += fabsf(inputVol[dataIdx]);
            nElem++;
          }
        }
      }
    }

    if (rfsaftabs > 0) {
      const float cf =
          flagCfWeight
              ? (rfsaft * rfsaft / (rfsaftabs * rfsaftabs * ((float)nElem)))
              : 1.0;
      outputVol[zIm + nT * (xIm + nX * yIm)] =
          ((float)signMultip) * rfsaft * cf;

    } else {
      outputVol[zIm + nT * (xIm + nX * yIm)] = 0;
    }
    // increase counter of reconstructed points
    atomicAdd((int*)progressCounter, 1);
    __threadfence_system();
  }

  return;
}

// run the kernel but on the CPU
void saft::saft_cpu() {

  // declare a few varaibles for improved readabilit
  const uint32_t nT = croppedData.get_dim(0);
  const uint32_t nX = croppedData.get_dim(1);
  const uint32_t nY = croppedData.get_dim(2);

  // for improved readability
  const float t0 = croppedData.get_origin(0);
  const float dt = croppedData.get_res(0);
  const float dx = croppedData.get_res(1);
  const float dy = croppedData.get_res(2);
  const float c0 = sett.get_flagUs() ? 0.5 * sett.get_sos() : sett.get_sos();
  const float fd = trans.get_focalDistance() * 1e-3;

  float* outputVol = reconData.get_pdata();
  const float* inputVol = croppedData.get_pdata();

  const float deltaZT =
      sqrt(trans.get_focalDistance() * trans.get_focalDistance() -
           trans.get_rAperture() * trans.get_rAperture());
  const float critRatio = trans.get_rAperture() / deltaZT;

  // outer loop defining that we do everything for each and every pixel
  for (uint32_t yIm = 0; yIm < nY; yIm++) {
    for (uint32_t xIm = 0; xIm < nX; xIm++) {
      for (uint32_t zIm = 0; zIm < nT; zIm++) {
        const float zDepth =
            (t0 + dt * (float)zIm) * c0; // depth of reconstructed point

        // 1 if far field, -1 if close field
        const float signMultip = -1 + 2 * ((float)(zDepth >= fd));

        const float deltaZ =
            zDepth - fd;     // z distance between focal point and reson point
        float rfsaft = 0;    // coherent saft sum
        float rfsaftabs = 0; // incoherent saft sum
        const int idxFoc = (fd / c0 - t0) / dt + 0.5; // t idx of focal point

        // limit reconstruction range to a local subfield
        const int yConsider =
            critRatio * fabsf(deltaZ) / dy; // index range to consider in y
        const int xConsider =
            critRatio * fabsf(deltaZ) / dx; // index range to consider in x
        const int yStartIdx = ((yIm - yConsider) <= 0) ? 0 : yIm - yConsider;
        const int xStartIdx = ((xIm - xConsider) <= 0) ? 0 : xIm - xConsider;
        const int yStopIdx =
            ((yIm + yConsider) >= nY) ? nY - 1 : yIm + yConsider;
        const int xStopIdx =
            ((xIm + xConsider) >= nX) ? nX - 1 : xIm + xConsider;

        int nElem = 0;

#pragma unroll
        for (int iY = yStartIdx; iY <= yStopIdx;
             iY++) // run over all other a scans in y
        {
          const float yRel = dy * (float)(iY - yIm);
#pragma unroll
          for (int iX = xStartIdx; iX <= xStopIdx;
               iX++) // run over all other a scans in x
          {
            const float xRel = dx * (float)(iX - xIm);
            const float rDist = sqrtf(xRel * xRel + yRel * yRel);
            const float currRatio = rDist / fabsf(deltaZ);

            if (currRatio <= critRatio) {
              // calculate distance, delay time, and index
              const float distTot = sqrtf(rDist * rDist + deltaZ * deltaZ);
              const int deltaT = distTot / c0 / dt + 0.5;
              const int tIdx = idxFoc + deltaT * signMultip;

              if ((tIdx >= 0) && (tIdx < nT)) {
                const int dataIdx = tIdx + nT * (iX + nX * iY);
                rfsaft += inputVol[dataIdx];
                rfsaftabs += fabsf(inputVol[dataIdx]);
                nElem++;
              }
            }
          }
        }

        if (rfsaftabs > 0) {
          const float cf =
              sett.get_flagCoherenceW()
                  ? rfsaft * rfsaft / (rfsaftabs * rfsaftabs * ((float)nElem))
                  : 1;
          outputVol[zIm + nT * (xIm + nX * yIm)] = signMultip * rfsaft * cf;

          // if (zDepth > fd)
          // 	outputVol[zIm + nT * (xIm + nX * yIm)] = rfsaft * cf;
          // else
          // 	outputVol[zIm + nT * (xIm + nX * yIm)] = -rfsaft * cf;
        } else {
          outputVol[zIm + nT * (xIm + nX * yIm)] = 0;
        }

        // update percDone
        const uint32_t nTotal = nT * nY * nX;
        const uint32_t nCurr = zIm + nT * (xIm + yIm * nX);
        percDone = ((float)nCurr) / ((float)nTotal) * 100;
      }
    }
  }

  return;
}

// start the actual reconstruction
void saft::recon() {
  isRunning = 1;
  crop();      // perform cropping of dataset to user defined boundaries
  remove_dc(); // remove dc component of each individual a scan in croppedData

  // push properties over to reconstructed dataset and allocate memory
  for (uint8_t iDim = 0; iDim < 3; iDim++) {
    reconData.set_dim(iDim, croppedData.get_dim(iDim));
    reconData.set_res(iDim, croppedData.get_res(iDim));
    reconData.set_origin(iDim, croppedData.get_origin(iDim));
  }

  // overwrite resolution and origin in z for us pulse echo mode
  const float dz = sett.get_flagUs()
                       ? croppedData.get_res(0) * sett.get_sos() / 2
                       : croppedData.get_res(0) * sett.get_sos();
  reconData.set_res(0, dz);
  const float originZ = sett.get_flagUs()
                            ? croppedData.get_origin(0) * sett.get_sos() / 2
                            : croppedData.get_origin(0) * sett.get_sos();
  reconData.set_origin(0, originZ);

  reconData.alloc_memory();                  // allocate memory
  tStart = std::chrono::system_clock::now(); // save start time to variable

  if (sett.get_flagGpu()) {
    // for improved readability
    const int nt = croppedData.get_dim(0);
    const int nx = croppedData.get_dim(1);
    const int ny = croppedData.get_dim(2);

    float* outputVol_dev;
    float* inputVol_dev;

    // allocate memory on GPU
    bool m1 = (cudaSuccess !=
               cudaMalloc((void**)&inputVol_dev,
                          croppedData.get_nElements() * sizeof(float)));
    m1 |= (cudaSuccess !=
           cudaMalloc((void**)&outputVol_dev,
                      croppedData.get_nElements() * sizeof(float)));
    if (m1) {
      printf("Could not allocate memory on GPU\n");
      throw "CudaMemAllocErr";
      return;
    }

    // copy preprocessed volume to gpu
    // copy signal matrix over to GPU and check if successful
    const bool cpy1 =
        (cudaSuccess != cudaMemcpy(inputVol_dev, croppedData.get_pdata(),
                                   croppedData.get_nElements() * sizeof(float),
                                   cudaMemcpyHostToDevice));
    if (cpy1) {
      printf("Could not copy array to GPU\n");
      throw "CudaMemCpyErr";
      return;
    }

    // define kernel size
    const dim3 blockSize(1, 16, 16);
    dim3 gridSize((nt + blockSize.x - 1) / blockSize.x,
                  (nx + blockSize.y - 1) / blockSize.y,
                  (ny + blockSize.z - 1) / blockSize.z);

    const float deltaZT =
        sqrt(trans.get_focalDistance() * trans.get_focalDistance() -
             trans.get_rAperture() * trans.get_rAperture());
    const float critRatio = trans.get_rAperture() / deltaZT;

    // if the dataset is ultrasound pulse echo measurement, just multiply sos
    // with 0.5
    const float reconSos =
        sett.get_flagUs() ? (sett.get_sos() * 0.5) : sett.get_sos();

    volatile int *d_progress, *h_progress;
    const bool map1 = (cudaSuccess != cudaSetDeviceFlags(cudaDeviceMapHost));
    if (map1) {
      printf("Something went wrong while mapping");
      throw "CudaError";
      return;
    }

    const bool m2 =
        (cudaSuccess !=
         cudaHostAlloc((void**)&h_progress, sizeof(int), cudaHostAllocMapped));
    if (m2) {
      printf("Something went wrong while allocating the progress integer");
      throw "CudaMemAllocErr";
      return;
    }

    const bool mPtr =
        (cudaSuccess !=
         cudaHostGetDevicePointer((int**)&d_progress, (int*)h_progress, 0));
    if (mPtr) {
      printf("Something went wrong while getting the device pointer");
      throw "CudaError";
      return;
    }

    *h_progress = 0;

    // execute actual kernel
    // all in SI units!
    SAFT<<<gridSize, blockSize>>>(
        outputVol_dev, (const float*)inputVol_dev, nt, nx, ny,
        croppedData.get_res(0), croppedData.get_res(1), croppedData.get_res(2),
        croppedData.get_origin(0),
        reconSos, // speed of sound in medium
        critRatio,
        trans.get_focalDistance() * 1e-3, // fical distance of transducer [m]
        sett.get_rMin(), sett.get_flagCoherenceW(), d_progress);

    int value = 0;
    do {
      int value1 = *h_progress;
      if (value1 > value) {
        // printf("h_progress = %d\n", value1);
        value = value1;
        percDone = ((float)value) / ((float)nx * ny * nt) * 100.0;
        auto tCurr = std::chrono::system_clock::now();
        const double tPassed =
            std::chrono::duration_cast<std::chrono::seconds>(tCurr - tStart)
                .count();
        tRemain = tPassed / percDone * (100.0 - percDone);
      }
    } while (value < (nt * nx * ny));

    cudaDeviceSynchronize();

    // check if kernel execution was successful
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
      printf("error during kernel execution:\n");
      printf(cudaGetErrorString(err));
      printf("\n");
      throw "CudaKernelCrash";
    }
    cudaMemcpy(reconData.get_pdata(), outputVol_dev,
               reconData.get_nElements() * sizeof(float),
               cudaMemcpyDeviceToHost);

    // free gpu memory again
    cudaFree(outputVol_dev);
    cudaFree(inputVol_dev);
    isRunning = 0;
  } else {
    isRunning = 1;
    saft_cpu();
    isRunning = 0;
  }

  tEnd = std::chrono::system_clock::now();
  reconTime =
      std::chrono::duration_cast<std::chrono::seconds>(tEnd - tStart).count();
  printf("[saft] reconstruction took %.1f seconds\n", reconTime);

  // calculate minimum and maximum value in reconstructed volume
  reconData.calcMinMax();
  reconData.calcMips();
  return;
}

// return a thread which runs the reconstruction
std::thread saft::recon2thread() {
  return std::thread([=] { recon(); });
}