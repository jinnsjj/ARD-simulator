#include "dct_volume.h"

DctVolume::DctVolume(int w, int h, int d) :width_(w), height_(h), depth_(d)
{
	values_ = (double*)calloc(width_*height_*depth_, sizeof(double));
	modes_ = (double*)calloc(width_*height_*depth_, sizeof(double));
	//memset(values_, 0, width_*height_*depth_ * sizeof(double));
	//memset(modes_, 0, width_*height_*depth_ * sizeof(double));

	dct_plan_ = fftw_plan_r2r_3d(depth_, height_, width_, values_, modes_, FFTW_REDFT10, FFTW_REDFT10, FFTW_REDFT10, FFTW_MEASURE);
	idct_plan_ = fftw_plan_r2r_3d(depth_, height_, width_, modes_, values_, FFTW_REDFT01, FFTW_REDFT01, FFTW_REDFT01, FFTW_MEASURE);

}

DctVolume::~DctVolume()
{
	fftw_destroy_plan(dct_plan_);
	fftw_destroy_plan(idct_plan_);
	free(values_);
	free(modes_);
}

void DctVolume::ExcuteDct()
{
	fftw_execute(dct_plan_);
	// FFTW3 does not normalize values, so we must perform this
	// step, or values will be wacky.
	for (int i = 0; i < depth_; i++)
	{
		for (int j = 0; j < height_; j++)
		{
			for (int k = 0; k < width_; k++)
			{
				modes_[i * height_ * width_ + j * width_ + k] /= 2.0 * sqrt(2.0 * depth_ * width_ * height_);
			}
		}
	}
}

void DctVolume::ExcuteIdct()
{
	fftw_execute(idct_plan_);
	// Normalization
	for (int i = 0; i < depth_; i++)
	{
		for (int j = 0; j < height_; j++)
		{
			for (int k = 0; k < width_; k++)
			{
				values_[i * height_ * width_ + j * width_ + k] /= sqrt(2.0 * depth_ * width_ * height_);
			}
		}
	}
}

double DctVolume::get_value(int x, int y, int z)
{
	return values_[z * height_ * width_ + y * width_ + x];
}

double DctVolume::get_mode(int x, int y, int z)
{
	return modes_[z * height_ * width_ + y * width_ + x];
}

void DctVolume::set_value(int x, int y, int z, double v)
{
	values_[z * height_ * width_ + y * width_ + x] = v;
}

void DctVolume::set_mode(int x, int y, int z, double m)
{
	modes_[z * height_ * width_ + y * width_ + x] = m;
}
