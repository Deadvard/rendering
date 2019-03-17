#include "renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <SDL/SDL.h>

void update(Camera* camera);
unsigned int createUniformBuffer(unsigned int size, int index);
void editUniformBuffer(unsigned int buffer, unsigned int size, void* data);

void renderer_init(Renderer * renderer)
{
	shader_init(&renderer->pbr, "resources/epic.vs", "resources/epic.fs");
	spheres_init(&renderer->spheres);
	renderer->UBO = createUniformBuffer(sizeof(Matrices), 0);

	renderer->camera.yaw = -90.0f;
}

void renderer_update(Renderer * renderer)
{
	update(&renderer->camera);
}

void renderer_render(Renderer* renderer)
{
	editUniformBuffer(renderer->UBO, sizeof(Matrices), &renderer->camera.mats);
	
	shader_use(&renderer->pbr);
	shader_setMat4(&renderer->pbr, "model", renderer->spheres.model[0]);
	
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

	float velocity = 1.0f;
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