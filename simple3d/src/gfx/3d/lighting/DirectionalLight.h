#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif // !GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>

#include "../../ShaderProgram.h"

class DirectionalLight
{
	friend class LightingManager;
private:
	glm::vec3 direction_;

	// Colors
	glm::vec3 ambient_	;
	glm::vec3 diffuse_	;
	glm::vec3 specular_	;

	GLboolean active_;
public:
	DirectionalLight();
	DirectionalLight(
		const glm::vec3& direction	,
		const glm::vec3& ambient	,
		const glm::vec3& diffuse	,
		const glm::vec3& specular
	);
	//DirectionalLight(const DirectionalLight&) = delete;
	//DirectionalLight& operator=(const DirectionalLight&) = delete;
	~DirectionalLight();
	
	GLboolean Active();

	// Setters
	GLvoid SetDirection(GLfloat x, GLfloat y, GLfloat z);
	GLvoid SetDirection(const glm::vec3& direction);
	GLvoid SetAmbient(GLfloat r, GLfloat g, GLfloat b);
	GLvoid SetDiffuse(GLfloat r, GLfloat g, GLfloat b);
	GLvoid SetSpecular(GLfloat r, GLfloat g, GLfloat b);
	GLvoid SetAmbient(const glm::vec3& ambient);
	GLvoid SetDiffuse(const glm::vec3& diffuse);
	GLvoid SetSpecular(const glm::vec3& specular);
	GLvoid SetActive(GLboolean active);
private:
	GLvoid UpdateShaders(const ShaderProgram& program, const GLuint light_id) const;
};

