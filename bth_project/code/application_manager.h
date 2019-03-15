#ifndef APPLICATION_MANAGER_H
#define APPLICATION_MANAGER_H

#include "window.h"

struct Game
{	
	Window window;
};

void run();
void update(Game* game);
void render(Game* game);

#endif