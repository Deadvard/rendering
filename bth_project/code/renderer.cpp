#include "renderer.h"

void renderer_init(Renderer * renderer)
{
	shader_init(&renderer->pbr, "epic.vs", "epic.fs");
}

void renderer_render(Renderer* renderer)
{
	shader_use(&renderer->pbr);
}

void update(Camera* camera)
{

}

unsigned int createUniformBuffer(Camera* camera)
{
	unsigned int buffer;

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Camera), 0, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 4, buffer);

	return buffer;
}

void editUniformBuffer(unsigned int buffer, Camera* camera)
{
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Camera), camera);
}