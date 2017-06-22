#version 330

in VertexData {
	vec3 color;
} i;

out vec4 color;

void main()	{
	color = vec4(i.color, 1.0f);
}