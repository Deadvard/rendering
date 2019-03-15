#ifndef APPLICATION_MANAGER_HPP
#define APPLICATION_MANAGER_HPP

#include <glad.c>
#include <SDL/SDL.h>

class ApplicationManager
{
public:
	void run();
private:
	void update();
	void draw();

};


#endif