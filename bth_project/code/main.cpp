#include <SDL/SDL.h>

int main(int argc, char** argv)
{
	SDL_Window* window = SDL_CreateWindow(
		"An SDL2 window", 
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,                    
		480,                    
		SDL_WINDOW_OPENGL);
	
	return 0;
}