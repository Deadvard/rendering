#ifndef WINDOW_HPP
#define WINDOW_HPP
#include <SDL/SDL.h>

struct Window
{
	SDL_Window* window;
	SDL_GLContext gl_context;
};

bool window_init(Window * window, const char* title, int width, int height);
void window_destroy(Window* window);

void window_clear();
void window_display(Window* window);

#endif