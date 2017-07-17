#version 330

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 tex_coords;
layout(location = 5) in vec3 color;

out VertexData {
	vec3 color;
	vec2 tex_coords;
} o;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
	gl_Position = u_projection * u_view * u_model * vec4(position, 1.0f);
	//gl_Position = vec4(position, 1.0f);
	o.color = color;
	o.tex_coords = tex_coords;
}