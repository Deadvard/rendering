#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>
#include "shader.h"
#include "spheres.h"
#include <SDL/SDL_image.h>

struct PointLights
{
	static const int num_lights = 16;
	glm::vec3 positions[num_lights];
};

struct Textures
{
	static const int num_texture_types = 2;
	static const int num_textures = 5;
	unsigned int tex_id[num_texture_types * num_textures];
	SDL_Surface* albedo[num_texture_types];
	SDL_Surface* normal[num_texture_types];
	SDL_Surface* metallic[num_texture_types];
	SDL_Surface* roughness[num_texture_types];
	SDL_Surface* ambient_occlusion[num_texture_types];
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
	float dt;
};

struct Renderer
{
	PointLights point_lights;
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