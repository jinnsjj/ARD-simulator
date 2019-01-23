#pragma once
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include "partition.h"

class Recorder
{
	int id_;
	int x_, y_, z_;
	int total_steps_;

	std::shared_ptr<Partition> part_;
	
	std::fstream output_;
	std::fstream response_;

public:
	Recorder(int x, int y, int z, int total_steps = 1000);
	~Recorder();

	void FindPartition(std::vector<std::shared_ptr<Partition>> partitions);
	void RecordField(int time_step = 0);
	void RecordResponse(int time_step = 0);

	static std::vector<std::shared_ptr<Recorder>> ImportRecorders(std::string path);

};

