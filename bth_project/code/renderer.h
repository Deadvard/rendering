#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>
#include "shader.h"

struct Renderer
{
	Shader pbr;
};

struct Camera
{
	glm::mat4 view;
	glm::mat4 projection;
};

void renderer_init(Renderer* renderer);
void renderer_render(Renderer* renderer);


void update(Camera* camera);
unsigned int createUniformBuffer(Camera* camera);
void editUniformBuffer(unsigned int buffer, Camera* camera);


#endif