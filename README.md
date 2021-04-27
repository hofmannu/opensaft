# opensaft
Synthetic aperture focusing technique (SAFT) for acoustic resolution optoacoustic microscopy.

This repository contains an implementation of the synthetic aperture focsuing technique purely written in C++ and CUDA. You need a CUDA capable device and Linux installed on your PC to run this. Installation instructions will soon be provided for ArchLinux as well as dataset specifications.

## Limitations

SAFT induces a strong absorption bias over depth. Absorbance "reconstructed" at different depths can therefore never be compared in a quantitative manner. It also tends to amplify low frequency signals which is the reason why most groups so far reconstruct different freqeucny bands independently (feature requires implementation).

## Literature
*  J. Turner, H. Estrada, D. Razansky: WSAFT
*  Lihong Wang: virtual detector and coherence factor weighting
*  Urs A. T. Hofmann: Burst mode
