#ifndef RENDERER_H
#define RENDERER_H
#include "shader.h"

struct Renderer
{
	Shader pbr;
};

void renderer_init(Renderer* renderer);
void renderer_render(Renderer* renderer);

#endif