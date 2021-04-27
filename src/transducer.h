/* 
	class representing a spherically focused ultrasound transducer
	Author: Urs Hofmann
	Mail: hofmannu@ethz.ch
	Date: 27.12.2020
*/

#include <math.h>

#ifndef TRANSDUCER_H
#define TRANSDUCER_H

class transducer
{

private:
	float focalDistance = 7; // focal distance of transducer [mm]
	float rAperture = 3.2; // radius of aperture of transducer [mm]
	float rHole = 0.5; // radius of central hole of transducer [mm]

public:
	float* get_pfocalDistance() {return &focalDistance;};
	float* get_prAperture() {return &rAperture;};
	float* get_rHole() {return &rHole;};

	void set_focalDistance(const float _focalDistance);
	void set_rAperture(const float _rAperture);
	void set_rHole(const float _rHole);

	// return actual values for transducer properties 
	float get_focalDistance() const {return focalDistance;};
	float get_rAperture() const {return rAperture;};
	float get_rHole() const {return rHole;};
	float get_theta() const {return asin(rAperture / focalDistance);};

};

#endif