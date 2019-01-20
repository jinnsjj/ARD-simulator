#include "pml_partition.h"



int PmlPartition::GetIndex(int x, int y, int z)
{
	if (x < 0 || x >= width_)
		return width_ * height_ * depth_;
	if (y < 0 || y >= height_)
		return width_ * height_ * depth_;
	if (z < 0 || z >= depth_)
		return width_ * height_ * depth_;
	return z * height_ * width_ + y * width_ + x;
}

PmlPartition::PmlPartition(int xs, int ys, int zs, int w, int h, int d)
	: Partition(xs, ys, zs, w, h, d)
{
}


PmlPartition::~PmlPartition()
{
}

void PmlPartition::Update()
{
}

double* PmlPartition::get_pressure_field()
{
	return p_;
}

double PmlPartition::get_pressure(int x, int y, int z)
{
	return p_[GetIndex(x,y,z)];
}

void PmlPartition::set_force(int x, int y, int z, double f)
{
	force_[GetIndex(x, y, z)] = f;
}
