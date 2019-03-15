#include "application_manager.hpp"

void ApplicationManager::run()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window = SDL_CreateWindow(
		"An SDL2 window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_OPENGL);

	bool running = true;

	while (running)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
			{
				running = false;
				break;
			}
			case SDL_KEYDOWN:
			{
				running = false;
				break;
			}
			}
		}

		update();
		draw();
	}
}

void ApplicationManager::update()
{

}

void ApplicationManager::draw()
{

}