/*
	synthetic aperture focusing technique

	reconstruction procedure

	Author: Urs Hofmann
	Mail: hofmannu@ethz.ch
	Date: 16.04.2021

*/


#pragma once

#include <cstdio>
#include <thread>
#include <pthread.h>
#include <chrono> // used to stop time which is required for execution
#include "Transducer.h"
#include "reconSettings.h"
#include "volume.h"

using namespace std;

struct dcData
{
	uint64_t iVecStart;
	uint64_t iVecEnd;
	uint64_t nT;
	uint64_t threadId;
	float* ptr;
};

class saft
{
private:
	Transducer trans; // settings of the used transducer
	reconSettings sett; // reconstruction settings
	volume preprocData; // preprocessed datasets
	volume reconData; // reconstructed datasets
	volume croppedData; // cropped dataset

	int processor_count = 1;
	bool isRunning = false; // flag indicating if reconstruction is currently running
	float percDone = 0.0f; // perc of reconstruction done so far [%]

	// all we need to time the execution
	std::chrono::time_point<std::chrono::system_clock> tStart; // start time
	std::chrono::time_point<std::chrono::system_clock> tEnd; // end time
	double tRemain = 0;
	double reconTime = 0; // time required for last reconstruction

public:
	// class constructor
	saft();

	Transducer* get_ptrans() {return &trans;};
	reconSettings* get_psett() {return &sett;};
	volume* get_ppreprocData() {return &preprocData;};
	volume* get_preconData() {return &reconData;};
	volume* get_pcroppedData() {return & croppedData;};

	void recon();
	std::thread recon2thread();

	void saft_cpu(); // cpu version of the kernel, used for debugging and if no GPU present
	void crop();
	void remove_dc();

	[[nodiscard]] double get_reconTime() const noexcept {return reconTime;};
	[[nodiscard]] double get_tRemain() const noexcept {return tRemain;};
	std::chrono::time_point<std::chrono::system_clock> get_tStart() const {return tStart;};

	[[nodiscard]] bool get_isRunning() const noexcept {return isRunning;};
	[[nodiscard]] float get_percDone() const noexcept {return percDone;};
};

