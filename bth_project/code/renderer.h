#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>
#include "shader.h"

struct Renderer
{
	Shader pbr;
};

struct Matrices
{
	glm::mat4 view;
	glm::mat4 projection;
};

struct Camera
{
	int x;
	int y;
	glm::vec3 position;
	float yaw;
	float pitch;
	Matrices mats;
};

void renderer_init(Renderer* renderer);
void renderer_render(Renderer* renderer);


void update(Camera* camera, int dX, int dY);
unsigned int createUniformBuffer(unsigned int size, int index);
void editUniformBuffer(unsigned int buffer, unsigned int size, void* data);


#endif