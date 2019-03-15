#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>
#include "shader.h"
#include "spheres.h"

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

struct Renderer
{
	Shader pbr;
	Spheres spheres;
	Camera camera;
	unsigned int UBO;
};

void renderer_init(Renderer* renderer);
void renderer_render(Renderer* renderer);
void renderer_update(Renderer* renderer);




#endif