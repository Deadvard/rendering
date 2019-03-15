#include "renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <SDL/SDL.h>

void update(Camera* camera, int dX, int dY);
unsigned int createUniformBuffer(unsigned int size, int index);
void editUniformBuffer(unsigned int buffer, unsigned int size, void* data);

void renderer_init(Renderer * renderer)
{
	shader_init(&renderer->pbr, "resources/epic.vs", "resources/epic.fs");
	spheres_init(&renderer->spheres);
	renderer->UBO = createUniformBuffer(sizeof(Matrices), 0);
}

void renderer_update(Renderer * renderer)
{
	int oldX = renderer->camera.x;
	int oldY = renderer->camera.y;
	SDL_GetMouseState(&renderer->camera.x, &renderer->camera.y);

	update(&renderer->camera, renderer->camera.x - oldX, renderer->camera.y - oldX);

	editUniformBuffer(renderer->UBO, sizeof(Matrices), &renderer->camera.mats);
}

void renderer_render(Renderer* renderer)
{
	shader_use(&renderer->pbr);
	shader_setMat4(&renderer->pbr, "model", renderer->spheres.model[0]);
	shader_setMat4(&renderer->pbr, "view", renderer->camera.mats.view);
	shader_setMat4(&renderer->pbr, "projection", renderer->camera.mats.projection);
	glBindVertexArray(renderer->spheres.vao[0]);
	glDrawElements(GL_TRIANGLE_STRIP, renderer->spheres.index_count[0], GL_UNSIGNED_INT, 0);
}

void update(Camera* camera, int dX, int dY)
{
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

	camera->mats.view = glm::lookAt(camera->position, camera->position + forward, glm::vec3(0.0f, 1.0f, 0.0f));
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