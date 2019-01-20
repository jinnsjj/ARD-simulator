#pragma once
#include <vector>
#include <string>
#include <memory>

class SoundSource
{
	int id_;
	int x_, y_, z_;
public:
	SoundSource(int x, int y, int z);
	~SoundSource();

	virtual double SampleValue(double t) = 0;

	static std::vector<std::shared_ptr<SoundSource>> ImportSources(std::string path);

	int x() {
		return x_;
	}
	int y() {
		return y_;
	}
	int z() {
		return z_;
	}

	friend class Simulation;
	friend class Partition;
};

