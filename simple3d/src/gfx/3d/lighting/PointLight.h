#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif // !GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>

#include "../../ShaderProgram.h"

class PointLight
{
	friend class LightingManager;
private:
	glm::vec3 position_;

	// Colors
	glm::vec3 ambient_;
	glm::vec3 diffuse_;
	glm::vec3 specular_;

	// Attenuation values
	GLfloat constant_;
	GLfloat linear_;
	GLfloat quadratic_;

	GLboolean active_;
public:
	PointLight();
	PointLight(
		const glm::vec3& position,
		const glm::vec3& ambient,
		const glm::vec3& diffuse,
		const glm::vec3& specular,
		GLfloat constant = 1.0f,
		GLfloat linear = 0.07f,
		GLfloat quadratic = 0.0002
	);
	~PointLight();

	glm::vec3 Position();
	GLvoid SetPosition(GLfloat x, GLfloat y, GLfloat z);
	GLvoid SetPosition(const glm::vec3 position);

	GLvoid SetAmbient	(GLfloat r, GLfloat g, GLfloat b);
	GLvoid SetDiffuse	(GLfloat r, GLfloat g, GLfloat b);
	GLvoid SetSpecular	(GLfloat r, GLfloat g, GLfloat b);
	GLvoid SetAllColor  (GLfloat r, GLfloat g, GLfloat b);
	GLvoid SetAmbient	(glm::vec3 color);
	GLvoid SetDiffuse	(glm::vec3 color);
	GLvoid SetSpecular	(glm::vec3 color);
	GLvoid SetAllColor	(glm::vec3 color);

	GLboolean Active();
	GLvoid SetActive(GLboolean active);
private:
	GLvoid PointLight::UpdateShaders(const ShaderProgram& program, const GLuint light_id) const;
};

