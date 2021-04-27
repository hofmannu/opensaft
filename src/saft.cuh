/* 
	syntehtic aperture focusing technique

	reconstruction procedure

	Author: Urs Hofmann
	Mail: hofmannu@ethz.ch
	Date: 16.04.2021

	
*/


#ifndef SAFT_H
#define SAFT_H

#include <cstdio>
#include <cuda.h>

#include "transducer.h"
#include "reconSettings.h"
#include "../lib/CVolume/src/volume.h"

class saft
{
private:
	transducer trans; // settings of the used transducer
	reconSettings sett; // reconstruction settings
	volume preprocData; // preprocessed datasets
	volume reconData; // reconstructed datasets
	volume croppedData; // cropped dataset
	
public:
	transducer* get_ptrans() {return &trans;};
	reconSettings* get_psett() {return &sett;};
	volume* get_ppreprocData() {return &preprocData;};
	volume* get_preconData() {return &reconData;};
	volume* get_pcroppedData() {return & croppedData;};

	void recon();
	void crop();
};

#endif