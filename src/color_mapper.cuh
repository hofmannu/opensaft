/*
	c++ tooolbox for simple mapping from numbers to colors
	Author: Urs Hofmann
	Mail: hofmannu@ethz.ch
	Date: 25.09.2020
*/

#ifndef COLOR_MAPPER_H
#define COLOR_MAPPER_H

#include <cstdio>
#include <cstdint>

class color_mapper
{
	private:
		float minVal = -1; // minimum value in colormap
		float maxVal = 1; // maximum value in colormap
		float maxAbsVal = 1;
		float span = 2;

		float minCol[4] = {0.0f, 0.0f, 0.0f, 0.00f}; // color assigned to min
		float maxCol[4] = {1.0f, 1.0f, 1.0f, 1.00f}; // color assigned to max
		float spanCol[4] = {0.0f, 0.0f, 0.0f, 0.00f};
	
		void calc_span_col();
		void calc_max_abs();

		uint8_t mapType = 0;
		// 0: linear rgb
		// 1: diverging linear rgb (crossing 0 at 0)
	public:
		// set function
		void set_maxVal(const float _maxVal);
		void set_minVal(const float _minVal);
		void set_minCol(const float* _minCol); // 4 element vector passed
		void set_maxCol(const float* _maxVal); // 4 element vector passed
		void set_mapType(const uint8_t _mapType);

		// get pointer to min and max value
		float* get_pmaxVal() {return &maxVal;};
		float* get_pminVal() {return &minVal;};
		float get_maxVal() const {return maxVal;};
		float get_minVal() const {return minVal;};
		float* get_pminCol() {return &minCol[0];};
		float* get_pmaxCol() {return &maxCol[0];};

		// transfer function
		void convert_to_map(const float* dataIn, const uint64_t nElem, unsigned char* dataOut) const;
		void convert_to_rgba(const float* dataIn, const uint64_t nElem, unsigned char* dataOut) const;
		void convert_to_divmap(const float* dataIn, const uint64_t nElem, unsigned char* dataOut) const;
};

#endif

