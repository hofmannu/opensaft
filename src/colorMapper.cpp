#include "colorMapper.h"
#include <math.h>

void color_mapper::set_maxVal(const float _maxVal){
	maxVal = _maxVal;
	calc_max_abs();
	return;
}

void color_mapper::set_minVal(const float _minVal){
	minVal = _minVal;
	calc_max_abs();
	return;
}

void color_mapper::set_minCol(const float* _minCol){
	for (uint8_t iCol = 0; iCol < 4; iCol++)
		minCol[iCol] = _minCol[iCol];

	calc_span_col();
	return;
}

void color_mapper::set_maxCol(const float* _maxCol){
	for (uint8_t iCol = 0; iCol < 4; iCol++)
		maxCol[iCol] = _maxCol[iCol];

	calc_span_col();
	return;
}

void color_mapper::calc_span_col()
{
	for (uint8_t idx = 0; idx < 4; idx++)
		spanCol[idx] = maxCol[idx] - minCol[idx];

	return;
}

void color_mapper::calc_max_abs()
{
	maxAbsVal = (fabsf(maxVal) > fabsf(minVal)) ? fabsf(maxVal) : fabsf(minVal);
	return;
}


void color_mapper::convert_to_rgba(
	const float* dataIn,
	const uint64_t nElem,
	unsigned char* dataOut) const
{
	
	float spanTemp = maxVal - minVal;
	float spanColTemp[4];
	for (unsigned int iCol = 0; iCol < 4; iCol++)
		spanColTemp[iCol] = maxCol[iCol] - minCol[iCol];

	// scale whole array to range from to 0 to 1
	float temp;
	for (uint64_t iElem = 0; iElem < nElem; iElem++){
		temp = (dataIn[iElem] - minVal) / spanTemp; // scale to [0 ... 1]
		
		// limit temp to 0 ... 1
		if (temp < 0)
			temp = 0;
		else if (temp > 1)
			temp = 1;

		#pragma unroll
		for (unsigned char iCol = 0; iCol < 4; iCol++)
			dataOut[iCol + iElem * 4] = 
				(minCol[iCol] + temp * spanColTemp[iCol]) * 255;
	}

	return;
}

// make diverging colormap centered around 0 or 1
void color_mapper::convert_to_divmap(
	const float* dataIn,
	const uint64_t nElem,
	unsigned char* dataOut
	) const
{
	// scale whole array to range from to 0 to 1
	float temp;
	for (uint64_t iElem = 0; iElem < nElem; iElem++){
		temp = abs(dataIn[iElem]) / maxAbsVal; // scale to [0 ... 1]
		if (temp > 1)
			temp = 1;

		if (dataIn[iElem] < 0)
		{
			#pragma unroll
			for (unsigned char iCol = 0; iCol < 4; iCol++)
			{
				dataOut[iCol + iElem * 4] = (1 - temp * minCol[iCol]) * 255;
			}

		}
		else
		{
			#pragma unroll
			for (unsigned char iCol = 0; iCol < 4; iCol++)
				dataOut[iCol + iElem * 4] = (1 - temp * maxCol[iCol]) * 255;
		}
		dataOut[4] = 255;
		
	}
	return;
}

void color_mapper::convert_to_map(
	const float* dataIn,
	const uint64_t nElem,
	unsigned char* dataOut) const
{
	if (mapType == 0)
		convert_to_rgba(dataIn, nElem, dataOut);
	else if (mapType == 1)
	{
		convert_to_divmap(dataIn, nElem, dataOut);
		// set default colors for diverging map

	}
	return;
}

void color_mapper::set_mapType(const uint8_t _mapType)
{
	mapType = _mapType;
	if (mapType == 1)
	{
		// set minVal to bright blue
		minCol[0] = 1.0; // r
		minCol[1] = 1.0; // g
		minCol[2] = 0.0; // b
		minCol[3] = 0.0; // alpha
		// set maxVal to bright red
		maxCol[0] = 0.0;
		maxCol[1] = 1.0;
		maxCol[2] = 1.0;
		maxCol[3] = 0.0;	
	}
	return;
}