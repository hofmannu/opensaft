#include "reconSettings.h"

void reconSettings::set_sos(const float _sos)
{
	sos = _sos;
}

void reconSettings::set_flagUs(const bool _flagUs)
{
	flagUs = _flagUs;
}

void reconSettings::set_cropTMin(const float tMin)
{
	cropMm[0] = tMin;
}
void reconSettings::set_cropTMax(const float tMax)
{
	cropMm[1] = tMax;
}
void reconSettings::set_cropXMin(const float xMin)
{
	cropMm[2] = xMin;
}
void reconSettings::set_cropXMax(const float xMax)
{
	cropMm[3] = xMax;
}

void reconSettings::set_cropYMin(const float yMin)
{
	cropMm[4] = yMin;
}

void reconSettings::set_cropYMax(const float yMax)
{
	cropMm[5] = yMax;
}

void reconSettings::set_cropT(const float tMin, const float tMax)
{
	cropMm[0] = tMin;
	cropMm[1] = tMax;
}

void reconSettings::set_cropX(const float xMin, const float xMax)
{
	cropMm[2] = xMin;
	cropMm[3] = xMax;
}

void reconSettings::set_cropY(const float yMin, const float yMax)
{
	cropMm[4] = yMin;
	cropMm[5] = yMax;
}

void reconSettings::set_crop(const uint8_t iDim, const float minVal, const float maxVal)
{
	cropMm[iDim * 2] = minVal;
	cropMm[iDim * 2 + 1] = maxVal;
}

// checks for each dimension if the larger cropping value is bigger then the smaller
// if not it swaps them 
void reconSettings::sortCropping()
{
	for (uint8_t iDim = 0; iDim < 3; iDim++)
	{
		if (cropMm[iDim * 2] > cropMm[iDim * 2 + 1])
		{
			const float backup = cropMm[iDim * 2];
			cropMm[iDim * 2] = cropMm[iDim * 2 + 1];
			cropMm[iDim * 2 + 1] = backup;
		}
	}
}

void reconSettings::set_rMin(const float _rMin)
{
	rMin = _rMin;
}