#ifndef APPLICATION_MANAGER_H
#define APPLICATION_MANAGER_H

#include "window.h"
#include "renderer.h"

struct Game
{	
	Window window;
	Renderer renderer;
};

void run();
void update(Game* game);
void render(Game* game);

#endif