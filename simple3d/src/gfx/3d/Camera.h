#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif // !GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "../../Window.h"
#include "../../JoystickManager.h"
#include "../ShaderProgram.h"

class Camera
{
private:
	Window* window_;

	glm::mat4 view_mat_;
	glm::mat4 projection_mat_;

	glm::vec3 position_;
	glm::vec3 front_;
	glm::vec3 up_;
	GLfloat pitch_ = 0.0f;
	GLfloat yaw_ = -90.0f;
	GLfloat movement_speed_ = 10.0f;
	GLfloat look_sensitivity_ = 0.05f;

	GLfloat lastx_, lasty_;
	GLfloat xoffset_, yoffset_;

	JoystickManager* joystick_;
	std::vector<GLfloat> joyaxes_;
	GLboolean joypresent_;

	GLboolean first_mouse_movement_ = GL_TRUE;

	GLfloat dt_	= 0.0f;
	GLfloat t_old_ = 0.0f;
public:
	Camera(Window* window);
	Camera() = delete; // TODO
	~Camera();

	glm::vec3 Position();
	glm::vec3 Direction();

	GLvoid Update();
	GLvoid UpdateShaders(const ShaderProgram& program);
private:
	GLvoid Init();

	GLvoid DoMovement();
	GLvoid DoLooking();

	// Update Class Matrices
	GLvoid UpdateViewMatrix();
	GLvoid UpdateProjectionMatrix();

	// Update Shader Uniforms
	GLvoid UpdateViewMatrixUniform(GLuint  view_matrix_uniform) const;
	GLvoid UpdateProjectionMatrixUniform(GLuint  projection_matrix_uniform) const;
	GLvoid UpdatePositionUniform(GLuint  view_position_uniform) const;

	GLvoid UpdateTime();
	GLvoid UpdateXY();
};

