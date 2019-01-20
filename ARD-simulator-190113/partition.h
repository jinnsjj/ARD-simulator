#pragma once
#include <string>
#include <vector>
#include <memory>
//#include "sound_source.h"

class Boundary;
class SoundSource;

class Partition
{
protected:
	double dh_;
	double dt_;
	double c0_;
	int x_start_, x_end_;
	int y_start_, y_end_;
	int z_start_, z_end_;
	int width_, height_, depth_;
	double absorption_{ 0.999 };
	struct Info
	{
		int id;
		int num_sources{ 0 };
		int num_boundaries{ 0 };
	} info_;

	std::vector<std::shared_ptr<SoundSource>> sources_;
	std::vector<std::vector<int>> right_free_borders_;
	std::vector<std::vector<int>> left_free_borders_;
	std::vector<std::vector<int>> top_free_borders_;
	std::vector<std::vector<int>> bottom_free_borders_;
	std::vector<std::vector<int>> front_free_borders_;
	std::vector<std::vector<int>> back_free_borders_;

	bool should_render{ true };
	
public:

	Partition(int xs, int ys, int zs, int w, int h, int d);
	~Partition();

	virtual void Update() = 0;

	virtual double* get_pressure_field() = 0;
	std::vector<double> get_xy_plane(int z);
	std::vector<double> get_yz_plane(int x);
	virtual double get_pressure(int x, int y, int z) = 0;
	virtual void set_force(int x, int y, int z, double f) = 0;
	virtual std::vector<double> get_xy_forcing_plane(int z);

	void AddBoundary(std::shared_ptr<Boundary> boundary);
	void AddSource(std::shared_ptr<SoundSource> source);
	static std::vector<std::shared_ptr<Partition>> ImportPartitions(std::string path);
	void Info();

	void ComputeSourceForcingTerms(double t);

	friend class Boundary;
	//friend class SoundSource;
	friend class Simulation;
	friend class Tools;
};

