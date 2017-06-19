#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif // !GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>

#include "../../ShaderProgram.h"

class SpotLight
{
	friend class LightingManager;
private:	
	glm::vec3 position_;
	glm::vec3 direction_;

	// Colors
	glm::vec3 ambient_;
	glm::vec3 diffuse_;
	glm::vec3 specular_;

	// Cutoff values
	GLfloat inner_cutoff_;
	GLfloat outer_cutoff_;

	// Attenuation values
	GLfloat constant_;
	GLfloat linear_;
	GLfloat quadratic_;

	GLboolean active_;
public:
	SpotLight();
	~SpotLight();

	glm::vec3 Position();
	GLvoid SetPosition(GLfloat x, GLfloat y, GLfloat z);
	GLvoid SetPosition(const glm::vec3& position);

	glm::vec3 Direction();
	GLvoid SetDirection(GLfloat x, GLfloat y, GLfloat z);
	GLvoid SetDirection(const glm::vec3& direction);

	GLboolean Active();
	GLvoid SetActive(GLboolean active);

	GLvoid SetAmbient(GLfloat r, GLfloat g, GLfloat b);
	GLvoid SetDiffuse(GLfloat r, GLfloat g, GLfloat b);
	GLvoid SetSpecular(GLfloat r, GLfloat g, GLfloat b);
	GLvoid SetAmbient(const glm::vec3& ambient);
	GLvoid SetDiffuse(const glm::vec3& diffuse);
	GLvoid SetSpecular(const glm::vec3& specular);

private:
	GLvoid UpdateShaders(const ShaderProgram& program, const GLuint light_id) const;
};

