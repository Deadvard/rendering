#ifndef SPHERE_H
#define SPHERE_H
#include <glm/glm.hpp>

struct Spheres
{
	static const int num_spheres = 1;
	glm::mat4 model[num_spheres];
	unsigned int vao[num_spheres];
	unsigned int vbo[num_spheres];
	unsigned int ebo[num_spheres];
	unsigned int index_count[num_spheres];
};

void spheres_init(Spheres* spheres);

#endif