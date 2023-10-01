#version 330

uniform mat4 u_projection;
uniform mat4 u_translation;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_uv;

out vec2 v_uv;

void main()
{
	gl_Position = u_projection * u_translation * vec4(in_position, 1.0);
	v_uv = in_uv;
}
