#pragma once
#include "sound_source.h"

class GaussianSource :public SoundSource
{
public:
	GaussianSource(int x, int y,int z);
	~GaussianSource();

	virtual double SampleValue(double t);
};

