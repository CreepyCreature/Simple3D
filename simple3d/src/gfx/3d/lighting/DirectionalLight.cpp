#include "DirectionalLight.h"

#include <iostream>

#define DEFAULT_DIRECTION 0.0f, -1.0f, 0.0f
#define DEFAULT_AMBIENT 0.1f, 0.1f, 0.1f
#define DEFAULT_DIFFUSE 0.7f, 0.7f, 0.7f
#define DEFAULT_SPECULAR 1.0f, 1.0f, 1.0f

// #define DEBUG

DirectionalLight::DirectionalLight()
{
	direction_ = glm::vec3(DEFAULT_DIRECTION);

	ambient_  = glm::vec3(0.1f);
	diffuse_  = glm::vec3(0.7f);
	specular_ = glm::vec3(1.0f);	

	active_ = GL_TRUE;
}

DirectionalLight::DirectionalLight
(
	const glm::vec3& direction	,
	const glm::vec3& ambient	,
	const glm::vec3& diffuse	,
	const glm::vec3& specular
) :
	direction_	(direction	),
	ambient_	(ambient	),
	diffuse_	(diffuse	),
	specular_	(specular	)
{
	active_ = GL_TRUE;
}


DirectionalLight::~DirectionalLight()
{
}

GLvoid DirectionalLight::UpdateShaders(const ShaderProgram& program, const GLuint light_id) const
{
	program.Bind();
	std::string structstr = "u_dir_lights[" + std::to_string(light_id) + "]";
	glUniform3f(
		glGetUniformLocation(program.ID(), 
			(structstr + ".direction").c_str()
		),
		direction_.x, direction_.y, direction_.z
	);
	glUniform3f(
		glGetUniformLocation(program.ID(),
			(structstr + ".ambient").c_str()
		),
		ambient_.r, ambient_.g, ambient_.b
	);
	glUniform3f(
		glGetUniformLocation(program.ID(),
			(structstr + ".diffuse").c_str()
		),
		diffuse_.r, diffuse_.g, diffuse_.b
	);
	glUniform3f(
		glGetUniformLocation(program.ID(),
			(structstr + ".specular").c_str()
		),
		specular_.r, specular_.g, specular_.b
	);
	glUniform1i(
		glGetUniformLocation(program.ID(),
			(structstr + ".is_active").c_str()
		),
		active_
	);
	program.Unbind();

#ifdef DEBUG
	std::cout << "Directional: "
		<< glGetUniformLocation(program.ID(), (structstr + ".direction"	).c_str()) << " "
		<< glGetUniformLocation(program.ID(), (structstr + ".ambient"	).c_str()) << " "
		<< glGetUniformLocation(program.ID(), (structstr + ".diffuse"	).c_str()) << " "
		<< glGetUniformLocation(program.ID(), (structstr + ".specular"	).c_str()) << " "
		<< glGetUniformLocation(program.ID(), (structstr + ".is_active"	).c_str()) << " "
		<< std::endl;
#endif // DEBUG

}

GLvoid DirectionalLight::SetActive(GLboolean active)
{
	active_ = active;
}

GLboolean DirectionalLight::Active()
{
	return active_;
}

GLvoid DirectionalLight::SetDirection(GLfloat x, GLfloat y, GLfloat z)
{
	direction_.x = x;
	direction_.y = y;
	direction_.z = z;
}
GLvoid DirectionalLight::SetDirection(const glm::vec3& direction)
{
	direction_ = direction;
}
GLvoid DirectionalLight::SetAmbient(GLfloat r, GLfloat g, GLfloat b)
{
	ambient_.r = r;
	ambient_.g = g;
	ambient_.b = b;
}
GLvoid DirectionalLight::SetDiffuse(GLfloat r, GLfloat g, GLfloat b)
{
	diffuse_.r = r;
	diffuse_.g = g;
	diffuse_.b = b;
}
GLvoid DirectionalLight::SetSpecular(GLfloat r, GLfloat g, GLfloat b)
{
	specular_.r = r;
	specular_.g = g;
	specular_.b = b;
}
GLvoid DirectionalLight::SetAmbient(const glm::vec3& ambient)
{
	ambient_ = ambient;
}
GLvoid DirectionalLight::SetDiffuse(const glm::vec3& diffuse)
{
	diffuse_ = diffuse;
}
GLvoid DirectionalLight::SetSpecular(const glm::vec3& specular)
{
	specular_ = specular;
}