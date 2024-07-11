/*
        class representing our interface including all imgui based in and
   outputs Author: Urs Hofmann Mail: mail@hofmannu.org Date: 27.12.2020
*/

#pragma once

// #include <GL/glew.h>
#include "ColorMapper.h"
#include "Transducer.h"
#include "reconSettings.h"
#include "saft.h"
#include "volume.h"
#include <GLFW/glfw3.h>
#include <ImGuiFileDialog.h>
#include <SDL2/SDL.h>
#include <chrono>
#include <ctime>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>
#include <thread>

class Interface {
public:
  Interface(); // class constructor

  void InitWindow(int *argcp, char **argv);

private:
  // all the pointers to things which we get from the saft main class
  saft recon;              // object for reconstruction
  std::thread reconThread; // reconstruction thread

  Transducer *trans;
  reconSettings *sett;
  // todo swithc this guy to unique_ptr
  volume *inputDataVol;
  volume *reconDataVol;

  // for input dataset vizualization
  // todo make slicer a separate class
  int currSliceZ = 0;
  int currSliceY = 0;
  ColorMapper inDataMapper;
  GLuint inDataTexture;
  GLuint inDataTextureSlice;

  GLuint inDataMipZ;
  GLuint inDataMipY;
  ColorMapper mipRawMapper;

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
  ColorMapper reconDataMapper;

  GLuint reconMipY; // object used for reconstructed mip along y normal
  GLuint reconMipZ;
  ColorMapper reconMipMapper;

  // cropping range which we apply to mips of reconstructed datasets
  float zCropRecon[2] = {0.0f, 1.0f};
  float xCropRecon[2] = {0.0f, 1.0f};
  float yCropRecon[2] = {0.0f, 1.0f};
  float zCropReconMm[2] = {0.0f, 1.0f};
  float xCropReconMm[2] = {0.0f, 1.0f};
  float yCropReconMm[2] = {0.0f, 1.0f};
  float zStretchRecon = 4.0f;

  void MainDisplayCode();

  void TransducerWindow(); // used to define transdcuer properties
  void SettingsWindow();   // window to define reconstruction settings
  void DataLoaderWindow(); // allows user to load a dataset from a file
  void ReconWindow();      // reconstruction part
  void SetupWorkspace(ImGuiID &dockspace_id);

  // flags which windows are supposed to be shown
  bool show_transducer_window = true;
  bool show_settings_window = true;
  bool show_data_loader = true;
  bool show_recon_window = true;

  bool isDataSetDefined = false;
  bool isReconDone = false;
  bool isReconRunning = false;

  const char *windowTitle = "opensaft";
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.10f); // bg color

  void ImImagesc(const float *data, const uint64_t sizex, const uint64_t sizey,
                 GLuint *out_texture, const ColorMapper myCMap);

  const char *m_windowTitle = "opensaft"; //!< title of the window created
  const Float4 m_clearColor{0.45f, 0.55f, 0.60f,
                            0.10f}; //!< bg color of the window
};