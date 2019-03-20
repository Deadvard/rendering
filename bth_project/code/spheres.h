#ifndef SPHERE_H
#define SPHERE_H
#include <glm/glm.hpp>

struct Spheres
{
	static const int num_spheres = 2;
	glm::mat4 model[num_spheres];
	unsigned int vao[num_spheres];
	unsigned int vbo[num_spheres];
	unsigned int ebo[num_spheres];
	unsigned int index_count[num_spheres];
};

void spheres_init(Spheres* spheres);


struct spherical_point
{
	float Radius;
	float Phi;
	float Theta;
};

static glm::vec3 DirectionVector(spherical_point P)
{
	glm::vec3 Result;
	Result.x = cos(P.Phi) * cos(P.Theta);
	Result.y = sin(P.Phi);
	Result.z = cos(P.Phi) * sin(P.Theta);
	return Result;
}

static glm::vec3 ToVector(spherical_point P)
{
	glm::vec3 Result;
	Result.x = P.Radius * cos(P.Phi) * cos(P.Theta);
	Result.y = P.Radius * sin(P.Phi);
	Result.z = P.Radius * cos(P.Phi) * sin(P.Theta);
	return Result;
}

#endif