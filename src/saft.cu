#include "saft.cuh"

/*
	main saft kernel

	todo:
	- implement boundary calculation
	- implement frequency band filtering
*/

__global__ void SAFT
(
	float* outputVol, // reconstructed output volume [it, ix, iy]
	const float* inputVol, // input signal matrix [it, ix, iy]
	const int nT, // number of elemetns in time
	const int nX, // number of elements along x
	const int nY, // number of elements along y
	const float dt, // resolution in time domain
	const float dx, // resolution along x [m]
	const float dy, // resolution along y [m]
	const float t0, // time of first index
	const float c0, // speed of sound in medium [m/s]
	const float critRatio, // critical ratio betwen rDist and deltaZ
	const float fd, // focal distance
	const float rMin // this is the minimum size accepted for any angle
	)
{
	// get index in output volume
	const int zIm = blockIdx.x * blockDim.x + threadIdx.x;
	const int xIm = blockIdx.y * blockDim.y + threadIdx.y;
	const int yIm = blockIdx.z * blockDim.z + threadIdx.z;

	if ((xIm < nX) && (yIm < nY) && (zIm < nT))
	{
		const float zDepth = (t0 + dt * (float) zIm) * c0; // depth of reconstructed point 
		
		// 1 if far field, -1 if close field
		const float signMultip = -1 + 2 * ((float) (zDepth >= fd));
		
		const float deltaZ = zDepth - fd; // z distance between focal point and reson point
		float rfsaft = 0; // coherent saft sum
		float rfsaftabs = 0; // incoherent saft sum
		const int idxFoc = (fd / c0 - t0) / dt + 0.5; // t idx of focal point

		// limit reconstruction range to a local subfield
		const int yConsider = critRatio * fabsf(deltaZ) / dy; // index range to consider in y
		const int xConsider = critRatio * fabsf(deltaZ) / dx; // index range to consider in x
		const int yStartIdx = ((yIm - yConsider) <= 0) ? 0 : yIm - yConsider;
		const int xStartIdx = ((xIm - xConsider) <= 0) ? 0 : xIm - xConsider;
		const int yStopIdx = ((yIm + yConsider) >= nY) ? nY - 1 : yIm + yConsider;
		const int xStopIdx = ((xIm + xConsider) >= nX) ? nX - 1 : xIm + xConsider;

		int nElem = 0;

		#pragma unroll
		for (int iY = yStartIdx; iY <= yStopIdx; iY++) // run over all other a scans in y
		{
			const float yRel = dy * (float) (iY - yIm);  
			#pragma unroll
			for (int iX = xStartIdx; iX <= xStopIdx; iX++) // run over all other a scans in x
			{
				const float xRel = dx * (float) (iX - xIm);
				const float rDist = sqrtf(xRel * xRel + yRel * yRel);
				const float currRatio = rDist / fabsf(deltaZ);

				if (currRatio <= critRatio)
				{
					// calculate distance, delay time, and index
					const float distTot = sqrtf(rDist * rDist + deltaZ * deltaZ);
					const int deltaT = distTot / c0 / dt + 0.5;
					const int tIdx = idxFoc + deltaT * signMultip; 
					
					if ((tIdx >= 0) && (tIdx < nT))
					{
						const int dataIdx = tIdx + nT * (iX + nX * iY);
						rfsaft += inputVol[dataIdx];
						rfsaftabs += fabsf(inputVol[dataIdx]);
						nElem++;
					} 
				}
			}
		}

		if (rfsaftabs > 0)
		{
			const float cf = rfsaft * rfsaft / 
				(rfsaftabs * rfsaftabs * ((float) nElem));
			outputVol[zIm + nT * (xIm + nX * yIm)] = signMultip * rfsaft * cf;
			
			// if (zDepth > fd)
			// 	outputVol[zIm + nT * (xIm + nX * yIm)] = rfsaft * cf;
			// else
			// 	outputVol[zIm + nT * (xIm + nX * yIm)] = -rfsaft * cf;
		}
		else
		{
			outputVol[zIm + nT * (xIm + nX * yIm)] = 0;
		}


	}

	return;
}

void saft::crop()
{
	// check if all max values are bigger then the minimum values, otherwise swap them
	sett.sortCropping();

	// find index closest to minimum cropping positions
	uint64_t idxMin[3];
	uint64_t idxMax[3];
	for (uint8_t iDim = 0; iDim < 3; iDim++)
	{
		idxMin[iDim] = preprocData.getIdx(sett.get_cropMin(iDim) * 1e-3, iDim);
		// printf("min idx along %d is %d \n", iDim, idxMin[iDim]);
		// find index closest to maximum cropping positions
		idxMax[iDim] = preprocData.getIdx(sett.get_cropMax(iDim) * 1e-3, iDim);
		// printf("max idx alomg %d is %d \n", iDim, idxMax[iDim]);
	}

	// push res origin and size over from original dataset
	for (uint8_t iDim = 0; iDim < 3; iDim++)
	{
		
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
	for (uint64_t iy = idxMin[2]; iy < idxMax[2]; iy++)
	{
		const uint64_t iyRel = iy - idxMin[2];
		#pragma unroll
		for (uint64_t ix = idxMin[1]; ix < idxMax[1]; ix++)
		{
			const uint64_t ixRel = ix - idxMin[1];
			#pragma unroll
			for (uint64_t it = idxMin[0]; it < idxMax[0]; it++)
			{
				const uint64_t itRel = it - idxMin[0];
				croppedData.set_value(itRel, ixRel, iyRel, preprocData.get_value(it, ix, iy));
				runIdx++;
			}
		}
	}

	return;
}

void saft::recon()
{
	// perform cropping of dataset to user defined boundaries
	crop();
	
	// for improved readability
	const int nt = croppedData.get_dim(0);
	const int nx = croppedData.get_dim(1); 
	const int ny = croppedData.get_dim(2);

	float* outputVol_dev;
	float* inputVol_dev;

	// allocate memory on GPU
	bool m1 = (cudaSuccess != cudaMalloc( (void**)&inputVol_dev, 
		croppedData.get_nElements() * sizeof(float) ));
	m1 |= (cudaSuccess != cudaMalloc( (void**)&outputVol_dev, 
		croppedData.get_nElements() * sizeof(float) ));
	if (m1)
	{
		printf("Could not allocate memory on GPU");
		throw "CudaMemAllocErr";
		return;
	}

	// copy preprocessed volume to gpu
	// copy signal matrix over to GPU and check if successful
	bool cpy1 = (cudaSuccess != cudaMemcpy(inputVol_dev, croppedData.get_pdata(), 
			croppedData.get_nElements() * sizeof(float), cudaMemcpyHostToDevice));
	if (cpy1)
	{
		printf("Could not copy array to GPU");
		throw "CudaMemCpyErr";	
		return;
	}

	// define kernel size
	dim3 blockSize(1, 16, 16);
	dim3 gridSize(
				(nt + blockSize.x - 1) / blockSize.x, 
				(nx + blockSize.y - 1) / blockSize.y, 
				(ny + blockSize.z - 1) / blockSize.z);

	const float deltaZT = sqrt(
		trans.get_focalDistance() * trans.get_focalDistance() -
		trans.get_rAperture() * trans.get_rAperture());
	const float critRatio = trans.get_rAperture() / deltaZT; 

	// execute actual kernel
	// all in SI units!
	SAFT<<<  gridSize, blockSize >>>
	(
		outputVol_dev,
		(const float*) inputVol_dev,
		nt, nx, ny,
		croppedData.get_res(0), croppedData.get_res(1), croppedData.get_res(2),
		croppedData.get_origin(0), 
		sett.get_sos(), // speed of sound in medium
		critRatio,
		trans.get_focalDistance() * 1e-3, // fical distance of transducer [m]
		sett.get_rMin()
		);
	cudaDeviceSynchronize();

	// check if kernel execution was successful
	cudaError_t err = cudaGetLastError();
	if (err != cudaSuccess)
	{
		printf("error during kernel execution:\n");
		printf(cudaGetErrorString(err));
		throw "CudaKernelCrash";
	}

	for (uint8_t iDim = 0; iDim < 3; iDim++)
	{
		reconData.set_dim(iDim, croppedData.get_dim(iDim));
		reconData.set_res(iDim, croppedData.get_res(iDim));
		reconData.set_origin(iDim, croppedData.get_origin(iDim));
	}
	reconData.set_res(0, croppedData.get_res(0) * sett.get_sos());
	reconData.set_origin(0, croppedData.get_origin(0) * sett.get_sos());
	
	reconData.alloc_memory();

	cudaMemcpy(reconData.get_pdata(), outputVol_dev, 
		reconData.get_nElements() * sizeof(float), cudaMemcpyDeviceToHost );

	// free gpu memory again
	cudaFree(outputVol_dev);
	cudaFree(inputVol_dev);

	// calculate minimum and maximum value in reconstructed volume
	reconData.calcMinMax();
	return;

}