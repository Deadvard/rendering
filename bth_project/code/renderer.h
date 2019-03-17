#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>
#include "shader.h"
#include "spheres.h"
#include <SDL/SDL_image.h>

struct Textures
{
	static const int num_textures = 2;
	unsigned int tex_id[num_textures * 5];
	SDL_Surface* albedo[num_textures];
	SDL_Surface* normal[num_textures];
	SDL_Surface* metallic[num_textures];
	SDL_Surface* roughness[num_textures];
	SDL_Surface* ambient_occlusion[num_textures];
};

struct Matrices
{
	glm::mat4 view;
	glm::mat4 projection;
};

struct Camera
{
	glm::vec3 position;
	float yaw;
	float pitch;
	Matrices mats;
};

struct Renderer
{
	Shader pbr;
	Textures textures;
	Spheres spheres;
	Camera camera;
	unsigned int UBO;
};

void renderer_init(Renderer* renderer);
void renderer_render(Renderer* renderer);
void renderer_update(Renderer* renderer);




#endif