#include "renderer.h"
#include <glm/gtc/matrix_transform.hpp>

void renderer_init(Renderer * renderer)
{
	shader_init(&renderer->pbr, "epic.vs", "epic.fs");
}

void renderer_render(Renderer* renderer)
{
	shader_use(&renderer->pbr);
}

void update(Camera* camera, float dX, float dY)
{
	float sensitivity = 0.05f;
	float xoffset = dX * sensitivity;
	float yoffset = dY * sensitivity;

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