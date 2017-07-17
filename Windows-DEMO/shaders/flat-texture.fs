#version 330

in VertexData {
	vec3 color;
	vec2 tex_coords;
} i;

uniform sampler2D u_texture;

out vec4 color;

void main()	{
	color = vec4(i.color, 1.0f) * texture(u_texture, i.tex_coords);
}