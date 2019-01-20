#pragma once
#include "partition.h"
#include "dct_volume.h"

class DctPartition : public Partition
{
	double lx2_, ly2_, lz2_;	// actual length ^2
	double *cwt_{ nullptr };	// cos(wt)
	double *w2_{ nullptr };		// w^2

	DctVolume pressure_;
	DctVolume force_;

	double *prev_modes_{ nullptr };
	double *next_modes_{ nullptr };

public:
	DctPartition(int xs, int ys, int zs, int w, int h, int d);
	~DctPartition();

	virtual void Update();

	virtual double* get_pressure_field();
	virtual double get_pressure(int x, int y, int z);
	virtual void set_force(int x, int y, int z, double f);
	virtual std::vector<double> get_xy_forcing_plane(int z);

	double get_force(int x, int y, int z);
	std::vector<double> get_xy_force_plane(int z);
	friend class Boundary;
};

