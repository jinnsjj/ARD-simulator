#include "simulation.h"
#include "partition.h"
#include "boundary.h"
#include "tools.h"
#include "sound_source.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iomanip>


Simulation::Simulation(std::vector<std::shared_ptr<Partition>> &partitions, std::vector<std::shared_ptr<SoundSource>> &sources)
	: partitions_(partitions), sources_(sources)
{

	// Find all shared boundaried of partitions
	for (int i = 0; i < partitions_.size(); i++)
	{
		auto part_a = partitions_[i];
		for (int j = i + 1; j < partitions_.size(); j++)
		{
			auto part_b = partitions_[j];
			auto boundary = Boundary::FindBoundary(part_a, part_b);
			if (boundary)
			{
				boundaries_.push_back(boundary);
				part_a->AddBoundary(boundary);
				part_b->AddBoundary(boundary);
				//Tools::Visualize2dVector(part_a->bottom_free_borders_);
				//Tools::Visualize2dVector(part_b->top_free_borders_);
			}
		}
	}

	// Add sources to corresponding partition
	for (auto partition : partitions_)
	{
		for (auto source : sources_)
		{
			if (source->x_ >= partition->x_start_ && source->x_ < partition->x_end_ &&
				source->y_ >= partition->y_start_ && source->y_ < partition->y_end_ &&
				source->z_ >= partition->z_start_ && source->z_ < partition->z_end_)
			{
				partition->AddSource(source);
			}
		}
	}


	/*------------- partitions includes pml partition --------------------------*/

	x_start_ = y_start_ = z_start_ = std::numeric_limits<int>::max();
	x_end_ = y_end_ = z_end_ = std::numeric_limits<int>::min();

	for (auto partition : partitions_)
	{
		x_start_ = std::min(x_start_, partition->x_start_);
		y_start_ = std::min(y_start_, partition->y_start_);
		z_start_ = std::min(z_start_, partition->z_start_);

		x_end_ = std::max(x_end_, partition->x_end_);
		y_end_ = std::max(y_end_, partition->y_end_);
		z_end_ = std::max(z_end_, partition->z_end_);
	}

	size_x_ = x_end_ - x_start_;
	size_y_ = y_end_ - y_start_;
	size_z_ = z_end_ - z_start_;

	info_.num_dct_partitions = partitions_.size();
	info_.num_boundaries = boundaries_.size();
	info_.num_sources = sources_.size();

	pixels_.assign(size_x_*size_y_, 0);
	ready_ = true;
}

Simulation::~Simulation()
{
}

void Simulation::Update()
{
	int time_step = time_step_++;
	std::cout << "#" << std::setw(5) << time_step << " : ";
	std::cout << std::to_string(sources_[0]->SampleValue(time_step)) << " ";
	for (auto partition : partitions_)
	{
		partition->ComputeSourceForcingTerms(time_step);
		//std::cout<<std::to_string(partition->get_pressure(0,4,2))
		partition->Update();
		std::cout << "update partition " << partition->info_.id << " ";
	}
	for (auto boundary : boundaries_)
	{
		boundary->ComputeForcingTerms();
	}
	std::cout << std::endl;

	{	// Visualization
		SDL_PixelFormat* fmt = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
		for (auto partition : partitions_)
		{
			if (!partition->should_render) continue;
			int x_offset = partition->x_start_ - x_start_;
			int y_offset = partition->y_start_ - y_start_;
			//std::vector<double> partition_xy = partition->get_xy_forcing_plane(2 / Simulation::dh_);
			std::vector<double> partition_xy = partition->get_xy_plane(2 / Simulation::dh_);
			for (int i = 0; i < partition->height_; i++) {
				for (int j = 0; j < partition->width_; j++) {
					double pressure = partition_xy[i*partition->width_ + j];
					double norm = 0.5*std::max(-1.0, std::min(1.0, pressure*1e9)) + 0.5;
					int r, g, b;
					if (norm >= 0.5)
					{
						r = static_cast<int> (255 - round(255.0*2.0*(norm - 0.5)));
						g = static_cast<int> (255 - round(255.0*2.0*(norm - 0.5)));
						b = 255;
					}
					else {
						r = 255;
						g = static_cast<int> (255 - round(255.0*(1.0 - 2.0*norm)));
						b = static_cast<int> (255 - round(255.0*(1.0 - 2.0*norm)));
					}
					pixels_[(y_offset + i)*size_x_ + (x_offset + j)] = SDL_MapRGBA(fmt, 255, r, g, b);
				}
			}
		}
	}

}

void Simulation::Info()
{
	std::cout << "# Simulation Info. #########################################" << std::endl;
	std::cout << "Simulation: "
		<< std::to_string(x_start_) << "," << std::to_string(y_start_) << "," << std::to_string(z_start_) << "->"
		<< std::to_string(x_end_) << "," + std::to_string(y_end_) << "," + std::to_string(z_end_) << std::endl;
	std::cout << "Size: " << "<" << size_x_ << "," << size_y_ << "," << size_z_ << ">" << std::endl;
	std::cout << "dh = " << std::to_string(Simulation::dh_)
		<< "(m), dt = " << std::to_string(Simulation::dt_)
		<< "(s), c0 = " << std::to_string(Simulation::c0_) << "(m/s)" << std::endl;
	std::cout << "Number of dct_partitions: " << info_.num_dct_partitions << std::endl;
	std::cout << "Number of boundaries: " << info_.num_boundaries << std::endl;
	std::cout << "Number of sources: " << info_.num_sources << std::endl;

	std::cout << "############################################################" << std::endl;
	for (auto p : partitions_)
	{
		p->Info();
	}
	std::cout << "------------------------------------------------------------" << std::endl;
	for (auto b : boundaries_)
	{
		b->Info();
	}
	std::cout << "------------------------------------------------------------" << std::endl;
	for (auto s : sources_)
	{
		std::cout << "Source " << s->id_ << ": " << s->x() << "," << s->y() << "," << s->z() << std::endl;
	}
	std::cout << "------------------------------------------------------------" << std::endl;
}

