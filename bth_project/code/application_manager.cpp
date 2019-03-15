#include "application_manager.h"

void run()
{
	Game game = {};
	window_init(&game.window, "APPLICATION MANAGER", 1280, 720);
	renderer_init(&game.renderer);

	SDL_SetRelativeMouseMode(SDL_TRUE);

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
				break;
			}
			}
		}


		update(&game);
		render(&game);
	}

	window_destroy(&game.window);
}

void update(Game* game)
{
	renderer_update(&game->renderer);
}

void render(Game* game)
{
	window_clear();

	renderer_render(&game->renderer);

	window_display(&game->window);
}