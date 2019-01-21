//#include <cstdio>
#include <iostream>

#include <SDL.h>
#undef main		// https://stackoverflow.com/questions/6847360

#include "simulation.h"
#include "partition.h"
#include "boundary.h"
#include "sound_source.h"
#include "gaussian_source.h"

using namespace std;

//double Simulation::dh_ = 0.05;
//double Simulation::dt_ = 0.5e-4;

double Simulation::dh_ = 0.1;
double Simulation::dt_ = 1e-4;

//double Simulation::dh_ = 0.5;
//double Simulation::dt_ = 6.25e-4;

double Simulation::c0_ = 3.435e2;
int Simulation::n_pml_layers_ = 10;

int main()
{
	//std::vector<std::shared_ptr<Partition>> partitions =
	//	Partition::ImportPartitions("./partitions-info-y.txt");
	//std::vector<std::shared_ptr<SoundSource>> sources =
	//	SoundSource::ImportSources("./sources-info-minus.txt");

	std::vector<std::shared_ptr<Partition>> partitions =
		Partition::ImportPartitions("./hall.txt");
	std::vector<std::shared_ptr<SoundSource>> sources =
		SoundSource::ImportSources("./hall-sources.txt");

	auto simulation = std::make_shared<Simulation>(partitions, sources);
	simulation->Info();
	//simulation->look_from_ = 1;

	SDL_Event event;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_PixelFormat* fmt = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	int resolution_x = 800;
	int resolution_y = resolution_x / simulation->size_x()*simulation->size_y();
	SDL_Window* window = SDL_CreateWindow("ARD Simulator",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, resolution_x, resolution_y, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING,
		simulation->size_x(), simulation->size_y());

	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&event)) {
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			}
		}
		simulation->Update();

		if (simulation->ready())
		{
			SDL_UpdateTexture(texture, nullptr,
				simulation->pixels().data(), simulation->size_x() * sizeof(Uint32));
		}
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, nullptr, nullptr);
		SDL_RenderPresent(renderer);
	}


	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

