#version 330

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 tex_coords;
layout(location = 5) in vec3 color;

out VertexData {
	vec3 color;
	vec2 tex_coords;
} o;

void main() {
	gl_Position  = vec4(position, 1.0f);

	o.color 	 = color;
	o.tex_coords = tex_coords;
}