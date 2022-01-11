/*
	class representing our interface including all imgui based in and outputs
	Author: Urs Hofmann
	Mail: hofmannu@ethz.ch
	Date: 27.12.2020
*/

#ifndef INTERFACE_H
#define INTERFACE_H

#include <SDL2/SDL.h>
#include <GL/glew.h>    // Initialize with gl3wInit()

#include <thread>
#include <ctime>
#include <chrono>

#include "../lib/imgui/imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "ImGuiFileDialog.h"
#include "imgui_plot.h"
#include "saft.cuh"
#include "transducer.h"
#include "reconSettings.h"
#include "../lib/CVolume/src/volume.h"
#include "color_mapper.cuh"

class interface
{
private:

	// all the pointers to things which we get from the saft main class
	saft recon; // object for reconstruction
	std::thread reconThread; // this is the thread in which we run the reconstruction

	transducer* trans;
	reconSettings* sett;
	volume* inputDataVol;
	volume* reconDataVol;

	// for input dataset vizualization
	int currSliceZ = 0;
	int currSliceY = 0;
	color_mapper inDataMapper;
	GLuint inDataTexture;
	GLuint inDataTextureSlice;

	GLuint inDataMipZ;
	GLuint inDataMipY;
	color_mapper mipRawMapper;

	// cropping range which we apply to mips of raw datasets
	float xCropRaw[2] = {0, 1};
	float zCropRaw[2] = {0, 1};
	float yCropRaw[2] = {0, 1};
	float xCropRawMm[2] = {0, 1};
	float zCropRawMm[2] = {0, 1};
	float yCropRawMm[2] = {0, 1};
	float zStretchRaw = 4; // how much should we stretch the preview in z

	// for output dataset vizualization
	int currSliceZRecon = 0;
	int currSliceYRecon = 0;
	GLuint reconSliceY; // crossection through reconstructed volume along y normal
	GLuint reconSliceZ; // crossection through reconstructed volume along z normal
	color_mapper reconDataMapper;

	GLuint reconMipY; // object used for reconstructed mip along y normal
	GLuint reconMipZ;
	color_mapper reconMipMapper;


	// cropping range which we apply to mips of reconstructed datasets
	float zCropRecon[2] = {0, 1};
	float xCropRecon[2] = {0, 1};
	float yCropRecon[2] = {0, 1};
	float zCropReconMm[2] = {0, 1};
	float xCropReconMm[2] = {0, 1};
	float yCropReconMm[2] = {0, 1};
	float zStretchRecon = 4;

	void MainDisplayCode();

	void TransducerWindow(); // used to define transdcuer properties
	void SettingsWindow(); // window to define reconstruction settings
	void DataLoaderWindow(); // allows user to load a dataset from a file
	void ReconWindow(); // reconstruction part

	// flags which windows are supposed to be shown
	bool show_transducer_window = 1;
	bool show_settings_window = 1;
	bool show_data_loader = 1;
	bool show_recon_window = 1;

	bool isDataSetDefined = 0;
	bool isReconDone = 0;
	bool isReconRunning = 0;

	const char* windowTitle = "opensaft";
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.10f); // bg color

	void ImImagesc(
	const float* data, const uint64_t sizex, const uint64_t sizey, 
	GLuint* out_texture, const color_mapper myCMap);

public:
	interface(); // class constructor

	void InitWindow(int *argcp, char**argv);


};

#endif