# opensaft

![documentation](https://github.com/hofmannu/opensaft/actions/workflows/deploy-documentation.yml/badge.svg)

> [!WARNING]
> This project is currently under development and not yet ready for use. Please check back later.

Volumetric synthetic aperture focusing technique (SAFT) for acoustic resolution optoacoustic microscopy and (soon) scanning acoustic microscopy.

This repository contains an implementation of the synthetic aperture focsuing technique purely written in C++ and CUDA. You need a CUDA capable device and Linux installed on your PC to run the fast GPU version of it. A "slow" CPU version is implemented as well. Installation instructions will soon be provided for ArchLinux as well as dataset specifications.

## Features

- Loading files from a stadardized h5 file format
- Defining transducer geometries
- Supports both ultrasound pulse echo and optoacoustic mode
- Graphical user interface including preview of preprocessed datasets and reconstructed datasets
- Fully open source and free of charge
- CUDA accelerated reconstruction procedure on GPU if requested

## Installation

Required packages:

- `cmake` to build compiling chain
- a capable C++ compiler which can be consumed by `CMake`
- `CUDA` for GPU acceleration (can be disabled, not required)
- `hdf5` for dataset import and export
- `libxrandr`, `libxinerama`, `libxcursor`, `libxi`, `libgl1-mesa-dev`: used for GUI framework

Package installation command ArchLinux:

```bash
pacman -S hdf5 libxrandr libxinerama
```

Package installation command Ubuntu:

```bash
apt-get install libhdf5-serial-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev
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

- Bandpass filtering directly within GUI and separate reconstruction into different normalized frequency bands as done in RSOM processing software
- Include sensitivity field building and weighting as done in WSAFT based on a GPU calculated model of the spherically focused detector
- Add a simple simulation toolbox to generate test datasets

## Limitations of the underlying model

SAFT induces a strong absorption bias over depth. Absorbance "reconstructed" at different depths can therefore never be compared in a quantitative manner. It also tends to amplify low frequency signals which is the reason why most groups so far reconstruct different freqeucny bands independently (feature requires implementation). In planes very distant from the focus blurring is unavoidable due to the large shape of the reconstruction arcs.

## Documentation

The documentation should be running with `vitepress` and is compiled into a static website that is also published to github pages. All documentation is written in markdown and can be found in the doc folder.

If this is the first time that you have a look at the documentation, you need to run

```bash
npm install
```

Afterwards, to display the documentation run

```bash
npm run docs:dev
```

To build the static website, run

```bash
npm run docs:build
```

The documentation is published on every merge to `main` to [github pages](https://hofmannu.github.io/opensaft/).

## Literature

- J. Turner et al.: Improved optoacoustic microscopy through three-dimensional spatial impulse response synthetic aperture focusing technique in Optics Letters 39 (12), pp. 3390 - 3393 (2014), [10.1364/OL.39.003390](https://doi.org/10.1364/OL.39.003390)
- J. Turner et al.: Universal weighted synthetic aperture focusing technique (W-SAFT) for scanning optoacoustic microscopy in Optica 4 (7), pp. 770 - 778 (2017), [10.1364/OPTICA.4.000770](https://doi.org/10.1364/OPTICA.4.000770)
- M.-L. Li et al.: Improved in vivo photoacoustic microscopy based on a virtual-detector concept in Optics Letters 31 (4), pp. 474 - 476 (2006), [10.1364/OL.31.000474](https://doi.org/10.1364/OL.31.000474)
- Urs A. T. Hofmann et al.: Rapid functional optoacoustic micro-angiography in a burst mode ion Optics Letters 45 (9), pp. 2522 - 2825 (2020), [10.1364/OL.387630](https://doi.org/10.1364/OL.387630)

## Known issue

### Error while loading files

If there is an error occuring about a version mismatch of the hdf5 library while loading a dataset from a file, it is most likely not the dataset itself that has an outdated format but rather the version of the installed h5 library and the library you linked during compilation (e.g. your PC updated the h5 library in the meantime). Simply recompile the program and the error should be gone.

### Currently not tested against datasets

I have no more access to optoacoustic microscopy images and therefore I am currently relying on simulation data to validate the performance of the algorithm. If you have experimental datasets which I can test the procedure against, feel free to contact me.