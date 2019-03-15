#version 430

in vertex_data
{
	vec3 world_pos;
	vec2 tex_coord;
	vec3 world_normal;
};

out vec4 frag_color;

void main()
{
	frag_color = vec4(1.0,0.5,1.0,1.0);
}