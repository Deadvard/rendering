#version 430

layout(location = 0) in vec3 position;

layout(binding = 0) uniform matrices
{
  mat4 projection;
  mat4 view;
};

uniform mat4 model;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1);
}