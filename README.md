# opensaft

Note: I am searching for test datasets of simple phantoms such as microspheres. Please contact me if you have some.

Volumetric synthetic aperture focusing technique (SAFT) for acoustic resolution optoacoustic microscopy and (soon) scanning acoustic microscopy.

This repository contains an implementation of the synthetic aperture focsuing technique purely written in C++ and CUDA. You need a CUDA capable device and Linux installed on your PC to run the fast GPU version of it. A "slow" CPU version is implemented as well. Installation instructions will soon be provided for ArchLinux as well as dataset specifications.

## Features

*  Loading files from a stadardized h5 file format
*  Defining transducer geometries
*  Supports both ultrasound pulse echo and optoacoustic mode
*  Graphical user interface including preview of preprocessed datasets and reconstructed datasets
*  Fully open source and free of charge
*  CUDA accelerated reconstruction procedure on GPU if requested

## Installation

Required packages:

*  `cmake` to build compiling chain
*  `make` to compile things
*  `CUDA` for GPU programming
*  `hdf5` for file import and export

Package installation command ArchLinux:

```bash
pacman -S hdf5
```

Package installation command Ubuntu:
```bash
apt-get install libhdf5-serial-dev
```

Many other dependencies are directly managed through CMake based on `FetchContent`.

Cloning and compiling

```bash
git clone https://github.com/hofmannu/opensaft
cd opensaft
mkdir build
cd build
cmake .. && make all
```

The last command will generate an executable called `main_exp`.

## Planned features
*  Bandpass filtering directly within GUI and separate reconstruction into different normalized frequency bands as done in RSOM processing software
*  Include sensitivity field building and weighting as done in WSAFT based on a GPU calculated model of the spherically focused detector

## Limitations of the underlying model

SAFT induces a strong absorption bias over depth. Absorbance "reconstructed" at different depths can therefore never be compared in a quantitative manner. It also tends to amplify low frequency signals which is the reason why most groups so far reconstruct different freqeucny bands independently (feature requires implementation). In planes very distant from the focus blurring is unavoidable due to the large shape of the reconstruction arcs.

## Coding conventions

- no `return` statement at the end of void functions

## Literature
*  J. Turner et al.: Improved optoacoustic microscopy through three-dimensional spatial impulse response synthetic aperture focusing technique in Optics Letters 39 (12), pp. 3390 - 3393 (2014), [10.1364/OL.39.003390](https://doi.org/10.1364/OL.39.003390)
*  J. Turner et al.: Universal weighted synthetic aperture focusing technique (W-SAFT) for scanning optoacoustic microscopy in Optica 4 (7), pp. 770 - 778 (2017), [10.1364/OPTICA.4.000770](https://doi.org/10.1364/OPTICA.4.000770)
*  M.-L. Li et al.: Improved in vivo photoacoustic microscopy based on a virtual-detector concept in Optics Letters 31 (4), pp. 474 - 476 (2006), [10.1364/OL.31.000474](https://doi.org/10.1364/OL.31.000474) 
*  Urs A. T. Hofmann et al.: Rapid functional optoacoustic micro-angiography in a burst mode ion Optics Letters 45 (9), pp. 2522 - 2825 (2020), [10.1364/OL.387630](https://doi.org/10.1364/OL.387630)

## Known issue

### Error while loading files

If there is an error occuring about a version mismatch of the hdf5 library while loading a dataset from a file, it is most likely not the dataset itself that has an outdated format but rather the version of the installed h5 library and the library you linked during compilation (e.g. your PC updated the h5 library in the meantime). Simply recompile the program and the error should be gone.