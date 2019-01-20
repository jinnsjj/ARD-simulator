#pragma once
#include <fftw3.h>
#include <memory>
#include <cmath>

class DctVolume
{
	int width_, height_, depth_;
	double *values_;
	double *modes_;

	fftw_plan dct_plan_;
	fftw_plan idct_plan_;

public:
	DctVolume(int w, int h, int d);
	~DctVolume();
	void ExcuteDct();
	void ExcuteIdct();
	double get_value(int x, int y, int z);
	double get_mode(int x, int y, int z);
	void set_value(int x, int y, int z, double v);
	void set_mode(int x, int y, int z, double m);

	friend class Partition;
	friend class DctPartition;
};

