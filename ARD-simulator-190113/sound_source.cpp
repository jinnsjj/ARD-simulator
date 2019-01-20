#include "sound_source.h"
#include "gaussian_source.h"
#include "simulation.h"
#include <fstream>

SoundSource::SoundSource(int x, int y, int z) :x_(x), y_(y), z_(z)
{
	static int id_generator = 0;
	id_ = id_generator++;
}


SoundSource::~SoundSource()
{
}

std::vector<std::shared_ptr<SoundSource>> SoundSource::ImportSources(std::string path)
{
	std::vector<std::shared_ptr<SoundSource>> sources;

	std::ifstream file;
	file.open(path, std::ifstream::in);
	while (file.good())
	{
		int x, y, z;
		file >> x >> y >> z;
		if (file.eof()) break;

		sources.push_back(std::make_shared<GaussianSource>(x / Simulation::dh_, y / Simulation::dh_, z / Simulation::dh_));
	}
	file.close();

	return sources;
}
