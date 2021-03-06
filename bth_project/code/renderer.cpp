#include "renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <SDL/SDL.h>

#define ArrayCount(array) (sizeof(array) / sizeof((array)[0]))
#define pi32 3.14159265359f

void update(Camera* camera);
unsigned int createUniformBuffer(unsigned int size, int index);
void editUniformBuffer(unsigned int buffer, unsigned int size, void* data);

static unsigned int textureFormat(Uint8 BytesPerPixel)
{
	if (BytesPerPixel == 4) return GL_RGBA;
	if (BytesPerPixel == 3) return GL_RGB;
	if (BytesPerPixel == 2) return GL_RG;
	return GL_RED;
}

void renderer_init(Renderer * renderer)
{
	spheres_init(&renderer->spheres);
	renderer->UBO = createUniformBuffer(sizeof(Matrices), 0);
	renderer->camera.yaw = -90.0f;

	for (int i = 0; i < renderer->point_lights.num_lights; i++)
	{
		renderer->spherical_points[i].Radius = 2.0f;
	}

	renderer->camera.is_lambert = true;
	
	renderer->textures.albedo[0] = IMG_Load("resources/textures/titanium/Titanium-Scuffed_basecolor.png");
	renderer->textures.normal[0] = IMG_Load("resources/textures/titanium/Titanium-Scuffed_normal.png");
	renderer->textures.metallic[0] = IMG_Load("resources/textures/titanium/Titanium-Scuffed_metallic.png");
	renderer->textures.roughness[0] = IMG_Load("resources/textures/titanium/Titanium-Scuffed_roughness.png");
	renderer->textures.ambient_occlusion[0] = IMG_Load("resources/textures/titanium/Titanium-Scuffed_ao.png");

	renderer->textures.albedo[1] = IMG_Load("resources/textures/iron/rustediron2_basecolor.png");
	renderer->textures.normal[1] = IMG_Load("resources/textures/iron/rustediron2_normal.png");
	renderer->textures.metallic[1] = IMG_Load("resources/textures/iron/rustediron2_metallic.png");
	renderer->textures.roughness[1] = IMG_Load("resources/textures/iron/rustediron2_roughness.png");
	renderer->textures.ambient_occlusion[1] = IMG_Load("resources/textures/iron/rustediron2_ao.png");

	for (int i = 0; i < renderer->textures.num_textures * renderer->textures.num_texture_types; i++)
	{
		glGenTextures(1, &renderer->textures.tex_id[i]);
		glBindTexture(GL_TEXTURE_2D, renderer->textures.tex_id[i]);
		int mode = GL_RGB;
		
		switch (i % 5)
		{
		case 0:
		{
			mode = textureFormat(renderer->textures.albedo[i == 0? 0 : 1]->format->BytesPerPixel);
			glTexImage2D(GL_TEXTURE_2D, 0, mode,
				renderer->textures.albedo[i == 0 ? 0 : 1]->w, renderer->textures.albedo[i == 0 ? 0 : 1]->h,
				0, mode, GL_UNSIGNED_BYTE, renderer->textures.albedo[i == 0 ? 0 : 1]->pixels);
			break;
		}
		case 1:
		{
			mode = textureFormat(renderer->textures.normal[i == 1 ? 0 : 1]->format->BytesPerPixel);
			glTexImage2D(GL_TEXTURE_2D, 0, mode,
				renderer->textures.normal[i == 1 ? 0 : 1]->w, renderer->textures.normal[i == 1 ? 0 : 1]->h,
				0, mode, GL_UNSIGNED_BYTE, renderer->textures.normal[i == 1 ? 0 : 1]->pixels);
			break;
		}
		case 2:
		{
			mode = textureFormat(renderer->textures.metallic[i == 2 ? 0 : 1]->format->BytesPerPixel);
			glTexImage2D(GL_TEXTURE_2D, 0, mode,
				renderer->textures.metallic[i == 2 ? 0 : 1]->w, renderer->textures.metallic[i == 2 ? 0 : 1]->h,
				0, mode, GL_UNSIGNED_BYTE, renderer->textures.metallic[i == 2 ? 0 : 1]->pixels);
			break;
		}
		case 3:
		{
			mode = textureFormat(renderer->textures.roughness[i == 3 ? 0 : 1]->format->BytesPerPixel);
			glTexImage2D(GL_TEXTURE_2D, 0, mode,
				renderer->textures.roughness[i == 3 ? 0 : 1]->w, renderer->textures.roughness[i == 3 ? 0 : 1]->h,
				0, mode, GL_UNSIGNED_BYTE, renderer->textures.roughness[i == 3 ? 0 : 1]->pixels);
			break;
		}
		case 4:
		{
			mode = textureFormat(renderer->textures.ambient_occlusion[i == 4 ? 0 : 1]->format->BytesPerPixel);
			glTexImage2D(GL_TEXTURE_2D, 0, mode,
				renderer->textures.ambient_occlusion[i == 4 ? 0 : 1]->w, renderer->textures.ambient_occlusion[i == 4 ? 0 : 1]->h,
				0, mode, GL_UNSIGNED_BYTE, renderer->textures.ambient_occlusion[i == 4 ? 0 : 1]->pixels);
			break;
		}
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	}
	shader_init(&renderer->pbr, "resources/shaders/epic.vs", "resources/shaders/epic.fs");
	shader_use(&renderer->pbr);
	shader_setInt(&renderer->pbr, "albedo_map", 0);
	shader_setInt(&renderer->pbr, "normal_map", 1);
	shader_setInt(&renderer->pbr, "metallic_map", 2);
	shader_setInt(&renderer->pbr, "roughness_map", 3);
	shader_setInt(&renderer->pbr, "ambient_occlusion_map", 4);

	for (int i = 0; i < renderer->point_lights.num_lights; i++)
	{
		renderer->point_lights.positions[i] = glm::vec3(0,0, -16.f);
	}

	renderer->spheres.model[1][3] = glm::vec4(5.f, 0.f, -20.f, 1.f);

}

void renderer_update(Renderer * renderer)
{
	update(&renderer->camera);

	for (int i = 0; i < ArrayCount(renderer->spherical_points); ++i)
	{
		renderer->spherical_points[i].Theta += renderer->camera.dt * pi32;
			
		renderer->point_lights.positions[i] = 
			glm::vec3(renderer->spheres.model[i][3]) + ToVector(renderer->spherical_points[i]);
	}
}

void renderer_render(Renderer* renderer)
{
	editUniformBuffer(renderer->UBO, sizeof(Matrices), &renderer->camera.mats);
	
	for (int i = 0; i < renderer->spheres.num_spheres; i++)
	{
		shader_use(&renderer->pbr);
		shader_setVec3(&renderer->pbr, "cam_pos", renderer->camera.position);
		shader_setMat4(&renderer->pbr, "model", renderer->spheres.model[i]);
		shader_setBool(&renderer->pbr, "is_lambert", renderer->camera.is_lambert);

		for (int i = 0; i < renderer->point_lights.num_lights; i++)
		{
			shader_setVec3(&renderer->pbr, "point_light_positions[" + std::to_string(i) + "]", renderer->point_lights.positions[i]);
			shader_setVec3(&renderer->pbr, "point_light_colors[" + std::to_string(i) + "]", glm::vec3(1,1,1));
		}
		if (i == 0)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, renderer->textures.tex_id[0]);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, renderer->textures.tex_id[1]);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, renderer->textures.tex_id[2]);

			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, renderer->textures.tex_id[3]);

			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, renderer->textures.tex_id[4]);
		}
		else
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, renderer->textures.tex_id[5]);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, renderer->textures.tex_id[6]);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, renderer->textures.tex_id[7]);

			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, renderer->textures.tex_id[8]);

			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, renderer->textures.tex_id[9]);
		}


		glBindVertexArray(renderer->spheres.vao[i]);
		glDrawElements(GL_TRIANGLE_STRIP, renderer->spheres.index_count[0], GL_UNSIGNED_INT, 0);
	}


}

void update(Camera* camera)
{
	int dX = 0;
	int dY = 0;
	SDL_GetRelativeMouseState(&dX, &dY);
	dY = -dY;

	float sensitivity = 0.05f;
	float xoffset = (float)dX * sensitivity;
	float yoffset = (float)dY * sensitivity;

	camera->yaw += xoffset;
	camera->pitch += yoffset;

	if (camera->pitch > 89.0f)
		camera->pitch = 89.0f;
	if (camera->pitch < -89.0f)
		camera->pitch = -89.0f;
	
	glm::vec3 forward;
	forward.x = glm::cos(glm::radians(camera->yaw)) * glm::cos(glm::radians(camera->pitch));
	forward.y = glm::sin(glm::radians(camera->pitch));
	forward.z = glm::sin(glm::radians(camera->yaw)) * glm::cos(glm::radians(camera->pitch));
	forward = glm::normalize(forward);

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	float velocity = 10.0f * camera->dt;
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);

	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_W]) direction.z += 1.0f;
	if (state[SDL_SCANCODE_A]) direction.x -= 1.0f;
	if (state[SDL_SCANCODE_S]) direction.z -= 1.0f;
	if (state[SDL_SCANCODE_D]) direction.x += 1.0f;

	if (state[SDL_SCANCODE_1]) camera->is_lambert = true;
	if (state[SDL_SCANCODE_2]) camera->is_lambert = false;

	camera->position += forward * velocity * direction.z;
	camera->position += glm::normalize(glm::cross(forward, up)) * velocity * direction.x;

	camera->mats.view = glm::lookAt(camera->position, camera->position + forward, up);
	camera->mats.projection = glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 1000.f);
}

unsigned int createUniformBuffer(unsigned int size, int index)
{
	unsigned int buffer;

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);
	glBufferData(GL_UNIFORM_BUFFER, size, 0, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, index, buffer);

	return buffer;
}

void editUniformBuffer(unsigned int buffer, unsigned int size, void* data)
{
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
}