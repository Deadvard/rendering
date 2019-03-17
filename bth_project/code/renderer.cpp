#include "renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <SDL/SDL.h>

void update(Camera* camera);
unsigned int createUniformBuffer(unsigned int size, int index);
void editUniformBuffer(unsigned int buffer, unsigned int size, void* data);

void renderer_init(Renderer * renderer)
{
	spheres_init(&renderer->spheres);
	renderer->UBO = createUniformBuffer(sizeof(Matrices), 0);
	renderer->camera.yaw = -90.0f;
	
	renderer->textures.albedo[0] = IMG_Load("resources/textures/bamboo/bamboo-wood-semigloss-albedo.png");
	renderer->textures.normal[0] = IMG_Load("resources/textures/bamboo/bamboo-wood-semigloss-normal.png");
	renderer->textures.metallic[0] = IMG_Load("resources/textures/bamboo/bamboo-wood-semigloss-metal.png");
	renderer->textures.roughness[0] = IMG_Load("resources/textures/bamboo/bamboo-wood-semigloss-roughness.png");
	renderer->textures.ambient_occlusion[0] = IMG_Load("resources/textures/bamboo/bamboo-wood-semigloss-ao.png");

	for (int i = 0; i < renderer->textures.num_textures; i++)
	{
		glGenTextures(1, &renderer->textures.tex_id[i]);
		glBindTexture(GL_TEXTURE_2D, renderer->textures.tex_id[i]);

		int mode = GL_RGB;
		switch (i)
		{
		case 0:
			if (renderer->textures.albedo[0]->format->BytesPerPixel == 4)
				mode = GL_RGB;
			glTexImage2D(GL_TEXTURE_2D, 0, mode, renderer->textures.albedo[0]->w, renderer->textures.albedo[0]->h, 0, mode, GL_UNSIGNED_BYTE, renderer->textures.albedo[0]->pixels);
			break;
		case 1:
			if (renderer->textures.normal[0]->format->BytesPerPixel == 4)
				mode = GL_RGB;
			glTexImage2D(GL_TEXTURE_2D, 0, mode, renderer->textures.normal[0]->w, renderer->textures.normal[0]->h, 0, mode, GL_UNSIGNED_BYTE, renderer->textures.normal[0]->pixels);
			break;
		case 2:
			if (renderer->textures.metallic[0]->format->BytesPerPixel == 4)
				mode = GL_RGBA;
			glTexImage2D(GL_TEXTURE_2D, 0, mode, renderer->textures.metallic[0]->w, renderer->textures.metallic[0]->h, 0, mode, GL_UNSIGNED_BYTE, renderer->textures.metallic[0]->pixels);
			break;
		case 3:
			if (renderer->textures.roughness[0]->format->BytesPerPixel == 4)
				mode = GL_RGB;
			glTexImage2D(GL_TEXTURE_2D, 0, mode, renderer->textures.roughness[0]->w, renderer->textures.roughness[0]->h, 0, mode, GL_UNSIGNED_BYTE, renderer->textures.roughness[0]->pixels);
			break;
		case 4:
			if (renderer->textures.ambient_occlusion[0]->format->BytesPerPixel == 4)
				mode = GL_RGB;
			glTexImage2D(GL_TEXTURE_2D, 0, mode, renderer->textures.ambient_occlusion[0]->w, renderer->textures.ambient_occlusion[0]->h, 0, mode, GL_UNSIGNED_BYTE, renderer->textures.ambient_occlusion[0]->pixels);
			break;
		}
		

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
		renderer->point_lights.positions[i] = glm::vec3(0,0, -5.f);
	}

}

void renderer_update(Renderer * renderer)
{
	update(&renderer->camera);
}

void renderer_render(Renderer* renderer)
{
	editUniformBuffer(renderer->UBO, sizeof(Matrices), &renderer->camera.mats);
	
	shader_use(&renderer->pbr);
	shader_setVec3(&renderer->pbr, "cam_pos", renderer->camera.position);
	shader_setMat4(&renderer->pbr, "model", renderer->spheres.model[0]);

	for (int i = 0; i < renderer->point_lights.num_lights; i++)
	{
		shader_setVec3(&renderer->pbr, "point_light_positions[0]", renderer->point_lights.positions[i]);
		shader_setVec3(&renderer->pbr, "point_light_colors[0]", glm::vec3(1,1,0));
	}

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


	glBindVertexArray(renderer->spheres.vao[0]);
	glDrawElements(GL_TRIANGLE_STRIP, renderer->spheres.index_count[0], GL_UNSIGNED_INT, 0);
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