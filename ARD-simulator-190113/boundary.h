#pragma once
#include <memory>
#include <string>
#include <iostream>

class Partition;

class Boundary
{
	int x_start_, x_end_;
	int y_start_, y_end_;
	int z_start_, z_end_;
	//std::string info_;
	struct Info
	{
		int id;
		int a_id;
		int b_id;
	}info_;
	
	std::shared_ptr<Partition> a_;
	std::shared_ptr<Partition> b_;

	double absorption_{ 1.0 };

public:


	enum BoundaryType {
		X_BOUNDARY,
		Y_BOUNDARY,
		Z_BOUNDARY
	} type_;

	Boundary(BoundaryType type, double absorp, std::shared_ptr<Partition> a, std::shared_ptr<Partition> b,
		int xs, int xe, int ys, int ye, int zs, int ze);
	~Boundary();

	void ComputeForcingTerms();

	static std::shared_ptr<Boundary> FindBoundary(std::shared_ptr<Partition> a, std::shared_ptr<Partition> b, double sbsorp = 1.0);
	void Info();

	friend class Partition;
};

