#include "recorder.h"
#include "simulation.h"
#include <iostream>


Recorder::Recorder(int x, int y, int z, int total_steps)
	: x_(x), y_(y), z_(z), total_steps_(total_steps)
{
	static int id_generator = 0;
	id_ = id_generator++;
	std::string filename;
	std::string dir_name = std::to_string(Simulation::dh_) + "_" + std::to_string(Partition::absorption_);
	filename = "./output/" + dir_name + "/out_" + std::to_string(id_) + ".txt";
	output_.open(filename, std::ios::out);
	filename = "./output/" + dir_name + "/response_" + std::to_string(id_) + ".txt";
	response_.open(filename, std::ios::out);
}


Recorder::~Recorder()
{
	output_.close();
	response_.close();
}

void Recorder::FindPartition(std::vector<std::shared_ptr<Partition>> partitions)
{
	for (auto partition : partitions)
	{
		if (partition->x_start_<x_ - 5 && partition->x_end_>x_ + 4 &&
			partition->y_start_<y_ - 5 && partition->y_end_>y_ + 4 &&
			partition->z_start_<z_ - 5 && partition->z_end_>z_ + 4)
		{
			part_ = partition;
			break;
		}
	}
}

void Recorder::RecordField(int time_step)
{
	if (time_step < total_steps_)
	{
		for (int i = -5; i < 5; i++)
		{
			for (int j = -5; j < 5; j++)
			{
				for (int k = -5; k < 5; k++)
				{
					output_ << part_->get_pressure(x_ + k, y_ + j, z_ + i) << " ";
				}
			}
		}
		output_ << std::endl;
		response_ << part_->get_pressure(x_, y_, z_) << std::endl;
	}
}

void Recorder::RecordResponse(int time_step)
{
	if (time_step <= total_steps_)
	{
		response_ << part_->get_pressure(x_, y_, z_) << std::endl;
	}
}

std::vector<std::shared_ptr<Recorder>> Recorder::ImportRecorders(std::string path)
{
	std::vector<std::shared_ptr<Recorder>> recorders;

	std::ifstream file;
	file.open(path, std::ifstream::in);
	while (file.good())
	{
		int x, y, z;
		file >> x >> y >> z;
		if (file.eof()) break;
		recorders.push_back(std::make_shared<Recorder>(x / Simulation::dh_, y / Simulation::dh_, z / Simulation::dh_, Simulation::duration_ / Simulation::dt_));
	}
	file.close();

	return recorders;
}
