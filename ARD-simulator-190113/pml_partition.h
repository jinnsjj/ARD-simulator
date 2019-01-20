#pragma once
#include "partition.h"

class PmlPartition : public Partition
{
	double* p_old_{ nullptr };
	double* p_{ nullptr };
	double* p_new_{ nullptr };

	double* phi_x_;
	double* phi_x_new_;
	double* phi_y_;
	double* phi_y_new_;
	double* phi_z_;
	double* phi_z_new_;
	double* force_;
	
	int GetIndex(int x, int y, int z);

public:
	PmlPartition(int xs, int ys, int zs, int w, int h, int d);
	~PmlPartition();

	virtual void Update();

	virtual double* get_pressure_field();
	virtual double get_pressure(int x, int y, int z);
	virtual void set_force(int x, int y, int z, double f);
};

