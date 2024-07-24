# Software

## Memory model

We have a container for the raw ultrasound signals coming in.
Thereby, each ultrasound waveform holds properties such as the 3D position, sampling rate etc.
Mutliple waveforms are then combined into an acquisition that also has properties which are shared between all the waveforms.

## Simulation

### Overview

The simulation is used to generate test datasets and to evaluate the influence of different physical effects. The idea is that in the beginning we start with a very simplistic world where there is only one single speed of sound in the medium, the transducer has an ideal frequency response, no dispersion of the wave is happening etc. Then we validate the reconstruction algorithm against this version of the world. After that we can add more and more physical effects to the simulation and see how the reconstruction algorithm performs.

The simulation is based on [finite difference time domain (FDTD) methods][ftdt-wiki] that was originally developed to solve the Maxwell equations for electromagnetic wave propagation. Nevertheless, the underlying differential equation is very similar and hence the method can be used to also describe ultrasound wave propagation. Both applications describe a problem where a temporal derivative of one field is related to the spatial derivative of another field.

Thereby, a simulation field is specified as a grid of voxels. At the beginning of the simulation, each voxel gets a material assigned. Each material has a defined density $\rho$ and a fourth order rigidity tensor $c$. Through those properties, the material is fully defined. Then boundary conditions are defined at the edges of the defined volume to allow the voxels to properly interact with the surrounding. The simulation then iterates over time and calculates the pressure field at each voxel based on the pressure field at the previous time step solving the wave equation. Sensors are placed in the simulation field to record the pressure field at each time step since storing the entire volume over the finely spaced time points would explode the memory consumption.

The pressure sources are defined as a set of points in the simulation field where a pressure wave is emitted. The pressure wave is defined by a waveform that is emitted during the first few timesteps of the simulation. In the most simplistic form of the algorithm, the pressure at the emission points is fixed and will be overwritten at any new timestep.

### Model equations

We simplify the source model in a way that we only allow forces sources meaning that during simulation for the start, the pressure at a few voxels will be defined through a source term and cannot be changed.

$$
\rho(x) \cdot \frac{\partial v_i}{\partial t}(x, t) = \sum_{j=1}^{3} \frac{\partial \sigma_{ij}}{\partial x_j}(x, t) + f(x, t)
$$

where

| Symbol | Description | Unit |
|--------|-------------|------|
| $\rho$ | Density | kg/m^3 |
| $v_i$ | Velocity | m/s |
| $\sigma_{ij}$ | Stress tensor | Pa |
| $f$ | Source term | N/m^3 |


Hooke's law:
$$
\frac{\partial \sigma_{ij}}{\partial t}(x, t) = \sum_{j=1}^d \sum_{i=1}^d c_{ijkl} (c) \frac{\partial v_k}{\partial x_l}(x, t)
$$

Any simulation that solves a partial differential equation then simplifies the derivative to be defined as the change over a small yet discrete step:

$$
\frac{\partial f}{\partial x} = \frac{f(x + \Delta x / 2) - f(x - \Delta x)}{\Delta x}
$$

where $\Delta x$ is the discretization of the problem. For the sake of the simulation we need to discretize both in temporal and spatial domain to solve the equation system. In the case of our simulation temporal discretization $\Delta t$ and spatial discretization $\Delta x$ are constant over the entire simulation field.

Basically this means, that for each temporal step we will visit all the neighboring voxels and calculate the update for the next step based on the current state of the simulation field. In a CUDA kernel this could be formulated as:

```cpp
// todo add a simple kernel here
```

### Meshing

#### Temporal mesh

#### Spatial mesh

We operate with a simple cubic mesh in which the differential equation is solved for each voxel. We iterate over all the neighboring voxels when calculating the gradient along 

### Settings

| Name | Description | Unit |


### Links

- [simsonic][simsonic]: a FTDT simulation of ultrasound propagation, written in C, difficult to read code
- [uftft tutorial][schneider2023]: a step-by-step guide about how one can build a FDTD toolbox for electromagnetics with chapter 12 focusing on acoustics 
- [Warren et al. 2019][warren2019]: a CUDA implementation of a FDTD algorithm for electromagnetic fields

## Reconstruction

[fdtd-wiki]: https://en.wikipedia.org/wiki/Finite-difference_time-domain_method
[boehm2017]: https://ethz.ch/content/dam/ethz/special-interest/erdw/geophysics/computational-seismology-dam/documents/Papers/Boehm_MUST_2017.pdf
[simsonic]: http://www.simsonic.fr/
[schneider2023][https://eecs.wsu.edu/~schneidj/ufdtd/ufdtd.pdf]
[warren2019]: https://www.sciencedirect.com/science/article/pii/S0010465518303990