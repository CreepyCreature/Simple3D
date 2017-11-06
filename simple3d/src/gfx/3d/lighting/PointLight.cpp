#include "PointLight.h"

#include <iostream>

#define DEFAULT_POSITION	0.0f, 0.0f, 0.0f

#define DEFAULT_AMBIENT		0.1f, 0.1f, 0.1f
#define DEFAULT_DIFFUSE		0.7f, 0.7f, 0.7f
#define DEFAULT_SPECULAR	1.0f, 1.0f, 1.0f

#define DEFAULT_CONSTANT	1.0f	
#define DEFAULT_LINEAR		0.07f	
#define DEFAULT_QUADRATIC	0.0002

// #define DEBUG

PointLight::PointLight()
{
	position_	= glm::vec3(DEFAULT_POSITION);

	ambient_	= glm::vec3(DEFAULT_AMBIENT	);
	diffuse_	= glm::vec3(DEFAULT_DIFFUSE	);
	specular_	= glm::vec3(DEFAULT_SPECULAR);

	constant_	= DEFAULT_CONSTANT	;
	linear_		= DEFAULT_LINEAR	;
	quadratic_	= DEFAULT_QUADRATIC	;

	active_ = GL_TRUE;
}

PointLight::PointLight
(
	const glm::vec3& position	,
	const glm::vec3& ambient	,
	const glm::vec3& diffuse	,
	const glm::vec3& specular	,
	GLfloat constant			,
	GLfloat linear				,
	GLfloat quadratic			
) :
	position_	(position	),
	ambient_	(ambient	),
	diffuse_	(diffuse	),
	specular_	(specular	),
	constant_	(constant	),
	linear_		(linear		),
	quadratic_	(quadratic	)
{
	active_ = GL_TRUE;
}

PointLight::~PointLight()
{
}

GLvoid PointLight::UpdateShaders(const ShaderProgram& program, const GLuint light_id) const
{
	program.Bind();
	std::string structstr = "u_point_lights[" + std::to_string(light_id) + "]";
	glUniform3f(
		glGetUniformLocation(program.ID(),
		(structstr + ".position").c_str()
		),
		position_.x, position_.y, position_.z
	);
	glUniform3f(
		glGetUniformLocation(program.ID(),
		(structstr + ".ambient").c_str()),
		ambient_.r, ambient_.g, ambient_.b
	);
	glUniform3f(
		glGetUniformLocation(program.ID(),
		(structstr + ".diffuse").c_str()),
		diffuse_.r, diffuse_.g, diffuse_.b
	);
	glUniform3f(
		glGetUniformLocation(program.ID(),
		(structstr + ".specular").c_str()),
		specular_.r, specular_.g, specular_.b
	);
	glUniform1f(
		glGetUniformLocation(program.ID(),
		(structstr + ".constant").c_str()),
		constant_
	);
	glUniform1f(
		glGetUniformLocation(program.ID(),
		(structstr + ".linear").c_str()),
		linear_
	);
	glUniform1f(
		glGetUniformLocation(program.ID(),
		(structstr + ".quadratic").c_str()),
		quadratic_
	);
	glUniform1i(
		glGetUniformLocation(program.ID(),
		(structstr + ".is_active").c_str()),
		active_
	);
	program.Unbind();

#ifdef DEBUG
	std::cout << "Point: "
		<< glGetUniformLocation(program.ID(), (structstr + ".position"	).c_str()) << " "
		<< glGetUniformLocation(program.ID(), (structstr + ".ambient"	).c_str()) << " "
		<< glGetUniformLocation(program.ID(), (structstr + ".diffuse"	).c_str()) << " "
		<< glGetUniformLocation(program.ID(), (structstr + ".specular"	).c_str()) << " "
		<< glGetUniformLocation(program.ID(), (structstr + ".constant"	).c_str()) << " "
		<< glGetUniformLocation(program.ID(), (structstr + ".linear"	).c_str()) << " "
		<< glGetUniformLocation(program.ID(), (structstr + ".quadratic"	).c_str()) << " "
		<< glGetUniformLocation(program.ID(), (structstr + ".is_active"	).c_str()) << " "
		<< std::endl;
#endif // DEBUG

}

glm::vec3 PointLight::Position()
{
	return position_;
}
GLvoid PointLight::SetPosition(GLfloat x, GLfloat y, GLfloat z)
{
	position_.x = x;
	position_.y = y;
	position_.z = z;
}
GLvoid PointLight::SetPosition(const glm::vec3 position)
{
	position_ = position;
}

GLvoid PointLight::SetActive(GLboolean active)
{
	active_ = active;
}
GLboolean PointLight::Active()
{
	return active_;
}


GLvoid PointLight::SetAmbient(GLfloat r, GLfloat g, GLfloat b)
{
	ambient_.r = r; ambient_.g = g; ambient_.b = b;
}
GLvoid PointLight::SetDiffuse(GLfloat r, GLfloat g, GLfloat b)
{
	diffuse_.r = r; diffuse_.g = g; diffuse_.b = b;
}
GLvoid PointLight::SetSpecular(GLfloat r, GLfloat g, GLfloat b)
{
	specular_.r = r; specular_.g = g; specular_.b = b;
}
GLvoid PointLight::SetAllColor(GLfloat r, GLfloat g, GLfloat b)
{
	SetAmbient(r, g, b); SetDiffuse(r, g, b); SetSpecular(r, g, b);
}
GLvoid PointLight::SetAmbient(glm::vec3 color)
{
	ambient_ = color;
}
GLvoid PointLight::SetDiffuse(glm::vec3 color)
{
	diffuse_ = color;
}
GLvoid PointLight::SetSpecular(glm::vec3 color)
{
	specular_ = color;
}

GLvoid PointLight::SetAllColor(glm::vec3 color)
{
	SetAmbient(color); SetDiffuse(color); SetSpecular(color);
}
