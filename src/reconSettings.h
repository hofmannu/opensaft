/* 
	reconstruction settings for saft
	Author: Urs Hofmann
	Mail: hofmannu@ethz.ch
	Date: 28.12.2020

	class defining the reconstruction settings for our SAFT procedure
*/

#ifndef RECONSETTINGS_H
#define RECONSETTINGS_H

#include <cstdint>

class reconSettings
{
private:
	float sos = 1495; // speed of sound used [m/s]
	bool flagCoherenceW = 1; // should we use coherence factor weighting
	bool flagSensW = 0; // should we also weight by sensitivity field
	bool flagPulseEcho = 0; // set to 1 for pulse echo, to 0 for optoacoustic
	float cropMm[6] = {0, 0, 0, 0, 0, 0	}; // crop applied in all three dimensions prior reconstruction
	float rMin = 50e-6; // minimum radius allowed for reconstruction arcs (overwrites angle values)

	bool flagUs = 0; // if enabled, pulse echo mode
	bool flagGpu = 1; // defines if the reconstruction should run on the GPU or CPU

public:
	// set and get functions for speed of sound
	float* get_psos() {return &sos;};
	float get_sos() const {return sos;};
	void set_sos(const float _sos);

	// flag for different logical settings
	bool* get_pflagCoherenceW() {return &flagCoherenceW;};
	bool get_flagCoherenceW() const {return flagCoherenceW;};

	bool* get_pflagSensW() {return &flagSensW;};
	bool* get_pflagPulseEcho() {return &flagPulseEcho;};
	bool get_flagPulseEcho() const {return flagPulseEcho;};
	void set_flagPulseEcho(const bool _flagPulseEcho) {flagPulseEcho = _flagPulseEcho; return;};

	// ultrasound pulse echo mode or optoacoustics switch
	bool* get_pflagUs() {return &flagUs;};
	void set_flagUs(const bool _flagUs); 
	bool get_flagUs() const {return flagUs;};

	bool* get_pflagGpu() {return &flagGpu;};
	bool get_flagGpu() const {return flagGpu;};
	void set_flagGpu(const bool flagGpu); 

	// defining and getting cropping
	float get_cropTMin() const {return cropMm[0];};
	float get_cropTMax() const {return cropMm[1];};
	float get_cropXMin() const {return cropMm[2];};
	float get_cropXMax() const {return cropMm[3];};
	float get_cropYMin() const {return cropMm[4];};
	float get_cropYMax() const {return cropMm[5];};

	void set_cropTMin(const float tMin);
	void set_cropTMax(const float tMax);
	void set_cropXMin(const float xMin);
	void set_cropXMax(const float xMax);
	void set_cropYMin(const float yMin);
	void set_cropYMax(const float yMax);

	void set_cropT(const float tMin, const float tMax);
	void set_cropX(const float xMin, const float xMax);
	void set_cropY(const float yMin, const float yMax);
	void set_crop(const uint8_t iDIm, const float minVal, const float maxVal);

	float get_cropMin(const uint8_t iDim) const {return cropMm[iDim * 2];};
	float get_cropMax(const uint8_t iDim) const {return cropMm[iDim * 2 + 1];};

	float* get_pcropT() {return &cropMm[0];};
	float* get_pcropX()	{return &cropMm[2];};
	float* get_pcropY() {return &cropMm[4];};

	float* get_prMin() {return &rMin;};
	float get_rMin() const {return rMin;};
	void set_rMin(const float _rMin);

	void sortCropping();
};

#endif