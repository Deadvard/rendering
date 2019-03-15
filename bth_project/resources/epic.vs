#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coordinate;
layout (location = 2) in vec3 normal;

out vertex_data
{
	vec3 world_pos;
	vec2 tex_coord;
	vec3 world_normal;
};

layout(binding = 0) uniform matrices
{
  mat4 projection;
  mat4 view;
};

uniform mat4 model;

void main()
{
	world_pos = (model * vec4(position, 1)).xyz;
    tex_coord = texture_coordinate;
	world_normal = (model * vec4(normal, 0)).xyz;

	gl_Position = projection * view * model * vec4(position, 1);
}