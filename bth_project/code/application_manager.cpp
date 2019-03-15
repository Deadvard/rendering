#include "application_manager.hpp"

void run()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window = SDL_CreateWindow(
		"An SDL2 window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_OPENGL);

	Game game = {};
	
	double timestep = 1.0 / 120.0;
	double last_time = (double)SDL_GetTicks() / 1000.0;
	double delta_time = 0.0;

	bool running = true;

	while (running)
	{
		double now = (double)SDL_GetTicks() / 1000.0;
		delta_time += now - last_time;
		last_time = now;
		
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

		update(&game);
		draw(&game);
	}
}

void update(Game* game)
{

}

void draw(Game* game)
{

}