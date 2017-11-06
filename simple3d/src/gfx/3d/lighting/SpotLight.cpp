#include "SpotLight.h"

#define DEFAULT_POSITION	0.0f, 0.0f, 0.0f
#define DEFAULT_DIRECTION	0.0f, 0.0f, -1.0f

#define DEFAULT_AMBIENT		0.1f, 0.1f, 0.1f
#define DEFAULT_DIFFUSE		0.7f, 0.7f, 0.7f
#define DEFAULT_SPECULAR	1.0f, 1.0f, 1.0f

#define DEFAULT_CONSTANT	1.0f	
#define DEFAULT_LINEAR		0.007f	
#define DEFAULT_QUADRATIC	0.0002

#define DEFAULT_INNER_CUTOFF	0.91
#define DEFAULT_OUTER_CUTOFF	0.82

SpotLight::SpotLight()
{
	position_	= glm::vec3(DEFAULT_POSITION);
	direction_	= glm::vec3(DEFAULT_DIRECTION);

	ambient_	= glm::vec3(DEFAULT_AMBIENT);
	diffuse_	= glm::vec3(DEFAULT_DIFFUSE);
	specular_	= glm::vec3(DEFAULT_SPECULAR);

	constant_	= DEFAULT_CONSTANT;
	linear_		= DEFAULT_LINEAR;
	quadratic_	= DEFAULT_QUADRATIC;

	inner_cutoff_ = DEFAULT_INNER_CUTOFF;
	outer_cutoff_ = DEFAULT_OUTER_CUTOFF;

	active_ = GL_TRUE;
}


SpotLight::~SpotLight()
{
}

glm::vec3 SpotLight::Position()
{
	return position_;
}

GLvoid SpotLight::SetPosition(GLfloat x, GLfloat y, GLfloat z)
{
	position_.x = x; position_.y = y; position_.z = z;
}

GLvoid SpotLight::SetPosition(const glm::vec3 & position)
{
	position_ = position;
}

glm::vec3 SpotLight::Direction()
{
	return direction_;
}

GLvoid SpotLight::SetDirection(GLfloat x, GLfloat y, GLfloat z)
{
	direction_.x = x; direction_.y = y; direction_.z = z;
}

GLvoid SpotLight::SetDirection(const glm::vec3 & direction)
{
	direction_ = direction;
}

GLboolean SpotLight::Active()
{
	return active_;
}

GLvoid SpotLight::SetActive(GLboolean active)
{
	active_ = active;
}

GLvoid SpotLight::SetAmbient(GLfloat r, GLfloat g, GLfloat b)
{
	ambient_.r = r;
	ambient_.g = g;
	ambient_.b = b;
}
GLvoid SpotLight::SetDiffuse(GLfloat r, GLfloat g, GLfloat b)
{
	diffuse_.r = r;
	diffuse_.g = g;
	diffuse_.b = b;
}
GLvoid SpotLight::SetSpecular(GLfloat r, GLfloat g, GLfloat b)
{
	specular_.r = r;
	specular_.g = g;
	specular_.b = b;
}
GLvoid SpotLight::SetAmbient(const glm::vec3& ambient)
{
	ambient_ = ambient;
}
GLvoid SpotLight::SetDiffuse(const glm::vec3& diffuse)
{
	diffuse_ = diffuse;
}
GLvoid SpotLight::SetSpecular(const glm::vec3& specular)
{
	specular_ = specular;
}

GLvoid SpotLight::UpdateShaders(const ShaderProgram & program, const GLuint light_id) const
{
	{
		program.Bind();
		std::string structstr = "u_spot_lights[" + std::to_string(light_id) + "]";
		glUniform3f(
			glGetUniformLocation(program.ID(),
			(structstr + ".position").c_str()
			),
			position_.x, position_.y, position_.z
		);
		glUniform3f(
			glGetUniformLocation(program.ID(),
			(structstr + ".direction").c_str()
			),
			direction_.x, direction_.y, direction_.z
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
			(structstr + ".inner_cutoff").c_str()),
			inner_cutoff_
		);
		glUniform1f(
			glGetUniformLocation(program.ID(),
			(structstr + ".outer_cutoff").c_str()),
			outer_cutoff_
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
			<< glGetUniformLocation(program.ID(), (structstr + ".position"		).c_str()) << " "
			<< glGetUniformLocation(program.ID(), (structstr + ".direction"		).c_str()) << " "
			<< glGetUniformLocation(program.ID(), (structstr + ".ambient"		).c_str()) << " "
			<< glGetUniformLocation(program.ID(), (structstr + ".diffuse"		).c_str()) << " "
			<< glGetUniformLocation(program.ID(), (structstr + ".specular"		).c_str()) << " "
			<< glGetUniformLocation(program.ID(), (structstr + ".inner_cutoff"	).c_str()) << " "
			<< glGetUniformLocation(program.ID(), (structstr + ".outer_cutoff"	).c_str()) << " "
			<< glGetUniformLocation(program.ID(), (structstr + ".constant"		).c_str()) << " "
			<< glGetUniformLocation(program.ID(), (structstr + ".linear"		).c_str()) << " "
			<< glGetUniformLocation(program.ID(), (structstr + ".quadratic"		).c_str()) << " "
			<< glGetUniformLocation(program.ID(), (structstr + ".is_active"		).c_str()) << " "
			<< std::endl;
#endif // DEBUG

	}
}
