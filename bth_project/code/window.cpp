#include "window.h"
#include <iostream>
#include <glad.c>

bool window_init(Window * window, const char* title, int width, int height)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	window->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, SDL_WINDOW_OPENGL);

	if (!window->window)
	{
		std::cout << "Unable to create window\n";
		std::cout << "SLD Error : " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetSwapInterval(1);

	window->gl_context = SDL_GL_CreateContext(window->window);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}
	glEnable(GL_DEPTH_TEST);

	return true;
}

void window_destroy(Window * window)
{
	SDL_GL_DeleteContext(window->window);
	SDL_DestroyWindow(window->window);
	SDL_Quit();
}

void window_clear()
{
	glClearColor(135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void window_display(Window* window)
{
	SDL_GL_SwapWindow(window->window);
}
