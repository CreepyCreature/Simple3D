#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coords;
layout(location = 3) in vec3 tangent;
//layout(location = 5) in vec3 color;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out vec3 Tangent;
//out vec3 Color;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()	{
	gl_Position = u_projection * u_view * u_model * vec4(position, 1.0f);
	//gl_Position = vec4(position, 1.0f); // Use this if you don't want to assign the MVP matrices
	Normal = mat3(transpose(inverse(u_model))) * normal;
	FragPos = vec3(u_model * vec4(position, 1.0f));
	TexCoords = tex_coords;
	Tangent = tangent;
	//Color = color;
}