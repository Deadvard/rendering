#ifndef APPLICATION_MANAGER_HPP
#define APPLICATION_MANAGER_HPP

#include "window.hpp"

struct Game
{	
	Window window;
};

void run();
void update(Game* game);
void render(Game* game);

#endif