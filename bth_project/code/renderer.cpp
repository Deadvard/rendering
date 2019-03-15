#include "renderer.h"

void renderer_init(Renderer * renderer)
{
	shader_init(&renderer->pbr, "epic.vs", "epic.fs");
}

void renderer_render(Renderer* renderer)
{
	shader_use(&renderer->pbr);
}
