#version 330

in VertexData {
	vec3 color;
	vec2 tex_coords;
} i;

out vec4 color;

uniform struct Material {
	sampler2D diffuse;
} material;

void main()	{
	color = texture(material.diffuse, i.tex_coords);
}