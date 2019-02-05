#pragma once
#include "partition.h"

class PmlPartition : public Partition
{
	std::shared_ptr<Partition> neighbor_part_;

	double R_{0.00001};
	double zeta_;
	double thickness_;

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

	double* zetax_;
	double* zetay_;
	double* zetaz_;
	
	// PML damping values
	double kxMin_{ 0.1 };
	double kxMax_{ 0.1 };
	double kyMin_{ 0.1 };
	double kyMax_{ 0.1 };
	double kzMin_{ 0.1 };
	double kzMax_{ 0.1 };

	int GetIndex(int x, int y, int z);

public:
	enum PmlType {
		P_LEFT,
		P_RIGHT,
		P_TOP,
		P_BOTTOM,
		P_FRONT,
		P_BACK
	}type_;

	PmlPartition(std::shared_ptr<Partition> neighbor_part, PmlType type, int xs, int ys, int zs, int w, int h, int d);
	~PmlPartition();

	virtual void Update();

	virtual double* get_pressure_field();
	virtual double get_pressure(int x, int y, int z);
	virtual void set_force(int x, int y, int z, double f);
};

