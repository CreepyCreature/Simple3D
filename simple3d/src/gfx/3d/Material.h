#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif // !GLEW_STATIC

#include <GL/glew.h>

#include <glm/glm.hpp>


struct Material
{
	glm::vec3 color_ambient;
	glm::vec3 color_diffuse;
	glm::vec3 color_specular;
	GLfloat shininess;
	GLfloat opacity;
};