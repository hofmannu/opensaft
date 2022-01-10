#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define TIME_INC 100000000
#define INCS 10
#define USE_PROGRESS 1
#define MAT_DIMX 4000
#define MAT_DIMY MAT_DIMX

#define cudaCheckErrors(msg) \
    do { \
        cudaError_t __err = cudaGetLastError(); \
        if (__err != cudaSuccess) { \
            fprintf(stderr, "Fatal error: %s (%s at %s:%d)\n", \
                msg, cudaGetErrorString(__err), \
                __FILE__, __LINE__); \
            fprintf(stderr, "*** FAILED - ABORTING\n"); \
            exit(1); \
        } \
    } while (0)

__global__ void mykernel(volatile int *data){

  unsigned long time;
  for (int i = 0; i < INCS; i++){
    atomicAdd((int *)data,1);
    __threadfence_system();
    time = clock64();
    while((clock64() - time)<TIME_INC) {};
    }
  printf("progress check finished\n");
}

__global__ void matmult(float *a, float *b, float *c, unsigned int rowA, unsigned int colA, unsigned int colB, volatile int *progress){
  unsigned int row = threadIdx.x+blockDim.x*blockIdx.x;
  unsigned int col = threadIdx.y+blockDim.y*blockIdx.y;
  if ((row < rowA) && (col < colB)){
    float temp = 0.0f;
    for (unsigned int k = 0; k < colA; k++)
      temp += a[(row*colA)+k] * b[(k*colB) + col];
    c[(row*colB)+col] = temp;
#if USE_PROGRESS
    if (!(threadIdx.x || threadIdx.y)){
      atomicAdd((int *)progress, 1);
      __threadfence_system();
      }
#endif
  }
}

int main(){
// simple test to demonstrate reading progress data from kernel
  volatile int *d_data, *h_data;
  cudaSetDeviceFlags(cudaDeviceMapHost);
  cudaCheckErrors("cudaSetDeviceFlags error");
  cudaHostAlloc((void **)&h_data, sizeof(int), cudaHostAllocMapped);
  cudaCheckErrors("cudaHostAlloc error");
  cudaHostGetDevicePointer((int **)&d_data, (int *)h_data, 0);
  cudaCheckErrors("cudaHostGetDevicePointer error");
  *h_data = 0;
  printf("kernel starting\n");
  mykernel<<<1,1>>>(d_data);
  cudaCheckErrors("kernel fail");
  int value = 0;
  do{
    int value1 = *h_data;
    if (value1 > value){
       printf("h_data = %d\n", value1);
       value = value1;}}
    while (value < (INCS-1));
  cudaDeviceSynchronize();
  cudaCheckErrors("kernel fail 2");

// now try matrix multiply with progress

  float *h_c, *d_a, *d_b, *d_c;
  h_c = (float *)malloc(MAT_DIMX*MAT_DIMY*sizeof(float));
  if (h_c == NULL) {printf("malloc fail\n"); return 1;}
  cudaMalloc((void **)&d_a, MAT_DIMX*MAT_DIMY*sizeof(float));
  cudaCheckErrors("cudaMalloc a fail");
  cudaMalloc((void **)&d_b, MAT_DIMX*MAT_DIMY*sizeof(float));
  cudaCheckErrors("cudaMalloc b fail");
  cudaMalloc((void **)&d_c, MAT_DIMX*MAT_DIMY*sizeof(float));
  cudaCheckErrors("cudaMalloc c fail");

  for (int i = 0; i < MAT_DIMX*MAT_DIMY; i++) h_c[i] = rand()/(float)RAND_MAX;
  cudaMemcpy(d_a, h_c, MAT_DIMX*MAT_DIMY*sizeof(float), cudaMemcpyHostToDevice);
  cudaCheckErrors("cudaMemcpy a fail");
  cudaMemcpy(d_b, h_c, MAT_DIMX*MAT_DIMY*sizeof(float), cudaMemcpyHostToDevice);
  cudaCheckErrors("cudaMemcpy b fail");

  cudaEvent_t start, stop;
  cudaEventCreate(&start); cudaEventCreate(&stop);
  *h_data=0;
  dim3 block(16,16);
  dim3 grid(((MAT_DIMX+block.x-1)/block.x), ((MAT_DIMY+block.y-1)/block.y));
  printf("matrix multiply kernel starting\n");
  cudaEventRecord(start);
  matmult<<<grid,block>>>(d_a, d_b, d_c, MAT_DIMY, MAT_DIMX, MAT_DIMX, d_data);
  cudaEventRecord(stop);
#if USE_PROGRESS
  unsigned int num_blocks = grid.x*grid.y;
  float my_progress = 0.0f;
  value = 0;
  printf("Progress:\n");
  do{
    cudaEventQuery(stop);  // may help WDDM scenario
    int value1 = *h_data;
    float kern_progress = (float)value1/(float)num_blocks;
    if ((kern_progress - my_progress)> 0.1f) {
      printf("percent complete = %2.1f\n", (kern_progress*100));
      my_progress = kern_progress;}}
    while (my_progress < 0.9f);
  printf("\n");
#endif
  cudaEventSynchronize(stop);
  cudaCheckErrors("event sync fail");
  float et;
  cudaEventElapsedTime(&et, start, stop);
  cudaCheckErrors("event elapsed time fail");
  cudaDeviceSynchronize();
  cudaCheckErrors("mat mult kernel fail");
  printf("matrix multiply finished.  elapsed time = %f milliseconds\n", et);


  return 0;
}