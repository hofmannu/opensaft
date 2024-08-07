# Physics

This document describes the physics simulated by the forward simulation toolbox which is part of this repository. The essence of the reconstruction algorithm is that it can only account for physical processes that are well described in the simulation.

This document is split into the description of the physics processes and the corresponding simulation I implemented. The forward description focuses on the clinical application of optoacoustics to image human skin and underlying tissue structures (if reachable). I on purpose try to give a broad image including the negative aspects and limitations of the imaging technology. In contrary to any research institute out there I do not depend on follow up grants and hence do not need to highlight positive aspects and hide limitations of the technology.

## Forward description

In optoacoustics, a nanosecond pulsed laser is used to illuminate the tissue of interest. The light propagates through the tissue layers where it interacts through scattering and absorption. Absorbed light is converted into heat that leads to thermal expansion and ultimately an ultrasound wave that propagates to the surface of the tissue where it is detected using a sensor.

The beauty of this imaging methodology is that theoretically it can combine optical contrast of the tissue with ultrasound resolution. It also has a very strong limitation that is the direct relationship between signal-to-noise ratio and penetration depth. Deep imaging can be achieved at low contrast and high contrast imaging is typically limited to a few millimeter.

### Optical modelling

The interaction between light and tissue is the most determining factor for the imaging depth. Often it is insufficiently highlighted that the imaging depth of optoacoustics is not limited by the ultrasound part but the optical light propagation. Hence assuming that optoacoustic imaging can reach as deep as ultrasound imaging is incorrect.

Especially in the visible range, human skin is designed to work against the penetration of light since the skin serves as a barrier to protect the underlying tissues. Skin is thereby highly scattering and depending on the melanin content potentially highly absorbing. This highlights another technical limitation of optoacoustics, being that light cannot penetrate different skin types and pigmentations equally well. The [absorption spectrum of melanin][omlc_melanin] decays towards higher wavelengths, potentially making the NIR and IR range less affected. Similarly, the other optical absorbers such as hemoglobin also show a decreased absorption in this range reducing the contrast of the imaging technology.

The optical modelling should be done using Monte Carlo simulations since the tissue is a highly scattering and absorbing medium. The light propagation can be described by the diffusion equation, but this is only valid for low scattering media. The diffusion equation is often used for the reconstruction of the optoacoustic signal, but it is not valid for the light propagation.

### Optoacoustic effect


### Acoustic modelling

Acoustic modelling describes the wave propagation from the moment the

#### Spatial impulse response

A transducer can consist out of multiple sensitive elements that might have a shape. 
For optoacoustic microscopy, often a single element is scanned over the surface, while for optoacoustic tomography, multiple elements are arranged in a static fashion next to each other.
Any transducer element can be modeled by subdividing the surface into a grid of small sensitive receiver units. For a simulation, the so called spatial impulse response of the transducer is calculated by calculation the distance distribution between field and the elements. 

As an amendment one can define the temporal impulse response for objects that have varying speed of sound. Thereby, along each path the distance is integrated over the speed of sound to calculate the delay time between the emission of the waveform and the reception.


[omlc_melanin]: https://omlc.org/spectra/melanin/mua.html