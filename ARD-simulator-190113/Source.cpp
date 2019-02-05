//#include <cstdio>
#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>
#include <omp.h>
#include <Windows.h>
#undef main		// https://stackoverflow.com/questions/6847360

#include "simulation.h"
#include "partition.h"
#include "boundary.h"
#include "sound_source.h"
#include "gaussian_source.h"
#include "recorder.h"

using namespace std;

bool is_record = true;

double Partition::absorption_ = 1.0;

double Simulation::duration_ = 1;

//double Simulation::dh_ = 0.05;
//double Simulation::dt_ = 0.625e-4;

double Simulation::dh_ = 0.1;
double Simulation::dt_ = 1.25e-4;

//double Simulation::dh_ = 0.2;
//double Simulation::dt_ = 2e-4;

//double Simulation::dh_ = 0.5;
//double Simulation::dt_ = 6.25e-4;

double Simulation::c0_ = 3.435e2;
int Simulation::n_pml_layers_ = 5;

int main()
{
	double time1 = omp_get_wtime();

	std::string dir_name = "./output/" + std::to_string(Simulation::dh_) + "_" + std::to_string(Partition::absorption_);
	CreateDirectory(dir_name.c_str(), NULL);

	std::vector<std::shared_ptr<Partition>> partitions;
	std::vector<std::shared_ptr<SoundSource>> sources;
	std::vector<std::shared_ptr<Recorder>> recorders;

	//partitions = Partition::ImportPartitions("./assets/scene-2.txt");
	//sources = SoundSource::ImportSources("./assets/sources.txt");

	partitions = Partition::ImportPartitions("./assets/hall.txt");
	sources = SoundSource::ImportSources("./assets/hall-sources.txt");
	recorders = Recorder::ImportRecorders("./assets/hall-recorders.txt");

	//partitions = Partition::ImportPartitions("./assets/classroom.txt");
	//sources = SoundSource::ImportSources("./assets/classroom-sources.txt");
	//recorders = Recorder::ImportRecorders("./assets/classroom-recorders.txt");

	for (auto record : recorders)
	{
		record->FindPartition(partitions);
	}

	auto simulation = std::make_shared<Simulation>(partitions, sources);
	simulation->Info();
	//simulation->look_from_ = 1;

	SDL_Event event;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_PixelFormat* fmt = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	int resolution_x = 800;
	int resolution_y = resolution_x / simulation->size_x()*simulation->size_y();
	SDL_Window* window = SDL_CreateWindow("ARD Simulator",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, resolution_x, resolution_y + 20, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING,
		simulation->size_x(), simulation->size_y());

	SDL_Rect simulation_rect;
	simulation_rect.x = 0;
	simulation_rect.y = 0;
	simulation_rect.w = resolution_x;
	simulation_rect.h = resolution_y;

	TTF_Init();
	TTF_Font* Sans = TTF_OpenFont("font/SourceSansPro-Regular.ttf", 64); //this opens a font style and sets a size
	SDL_Color White = { 255, 255, 255 };  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

	SDL_Rect Message_rect; //create a rect
	Message_rect.x = 0;  //controls the rect's x coordinate 
	Message_rect.y = resolution_y; // controls the rect's y coordinte
	Message_rect.w = 100; // controls the width of the rect
	Message_rect.h = 20; // controls the height of the rect
	SDL_Rect Message_rect2; //create a rect
	Message_rect2.x = resolution_x - 100;  //controls the rect's x coordinate 
	Message_rect2.y = resolution_y; // controls the rect's y coordinte
	Message_rect2.w = 100; // controls the width of the rect
	Message_rect2.h = 20; // controls the height of the rect

	bool quit = false;
	int time_step = 0;
	int total_time_steps = Simulation::duration_ / Simulation::dt_;
	std::string message;

	double time2 = omp_get_wtime();
	std::cout << "Initialization finished. (" << time2 - time1 << " s)" << std::endl;
	std::cout << "############################################################" << std::endl;

	while (!quit && time_step < total_time_steps)
	{
		while (SDL_PollEvent(&event)) {
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			}
		}
		//double start = omp_get_wtime();
		time_step = simulation->Update();
		//double end = omp_get_wtime();
		//std::cout << end - start << std::endl;
		if (is_record)
		{
			for (auto record : recorders)
			{
				record->RecordField(time_step);
			}
		}

		message = std::to_string(time_step) + '/' + std::to_string(total_time_steps);
		SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, message.c_str(), White);	// as TTF_RenderText_Solid could only be used on SDL_Surface then you haurfavesve to create the surface first
		SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

		//std::cout << "\r[----------------------------------------]" << "[" << time_step << "/" << total_time_steps << "]";
		//std::cout << "\r[";
		//double perProgress = 40.0 * (time_step) / total_time_steps;
		//while (perProgress-- > 0)
		//{
		//	std::cout << "#";
		//}

		if (simulation->ready())
		{
			SDL_UpdateTexture(texture, nullptr,
				simulation->pixels().data(), simulation->size_x() * sizeof(Uint32));
		}
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, nullptr, &simulation_rect);
		SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

		message = std::to_string((omp_get_wtime() - time1) / 60) + " min";
		surfaceMessage = TTF_RenderText_Solid(Sans, message.c_str(), White);
		Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
		SDL_RenderCopy(renderer, Message, NULL, &Message_rect2);

		SDL_FreeSurface(surfaceMessage);
		SDL_DestroyTexture(Message);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	double time3 = omp_get_wtime();
	std::cout << std::endl << "Simulation finished. (" << time3 - time1 << " s)" << std::endl;
	std::cout << "############################################################" << std::endl;

	return 0;
}

