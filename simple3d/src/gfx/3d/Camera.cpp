#include "Camera.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

Camera::Camera(Window* window) : 
	window_(window)
{
	position_	= glm::vec3(0.0f, 0.0f,  10.0f)	;
	front_		= glm::vec3(0.0f, 0.0f, -1.0f )	;
	up_			= glm::vec3(0.0f, 1.0f,  0.0f )	;

	pitch_	= 0.0f;
	yaw_ = -90.f; // Initialize yaw to -90 degrees in order to position the front at (0, 0, -1)

	movement_speed_		= 40.0f;
	look_sensitivity_	= 0.05f;

	Init();
}

GLvoid Camera::Init()
{
	yoffset_ = 0.0f;
	xoffset_ = 0.0f;
	first_mouse_movement_ = GL_TRUE;

	GLint width  = (*window_).FramebufferWidth();
	GLint height = (*window_).FramebufferHeight();

	view_mat_ = glm::lookAt(position_, position_ + front_, up_);
	projection_mat_ = glm::perspective
	(
		glm::radians(70.f),					// fovy
		((GLfloat)width / (GLfloat)height), // aspect ratio
		//1.0f, 100.f						// znear, zfar
		0.5f, 1000.0f
	);

	// Joystick handling
	joystick_ = JoystickManager::Instance();
	if ((*joystick_).JoystickPresent(JoystickManager::JOYSTICK_1))
	{
		joyaxes_ = (*joystick_).GetJoystickAxes(JoystickManager::JOYSTICK_1);
		joypresent_ = GL_TRUE;
	}
	else
	{
		joypresent_ = GL_FALSE;
	}

	t_old_ = glfwGetTime();
}

Camera::~Camera()
{
}

glm::vec3 Camera::Position()
{
	return position_;
}

glm::vec3 Camera::Direction()
{
	return front_;
}

GLvoid Camera::Update()
{
	if ((*joystick_).JoystickPresent(JoystickManager::JOYSTICK_1))
	{
		joyaxes_ = (*joystick_).GetJoystickAxes(JoystickManager::JOYSTICK_1);
		joypresent_ = GL_TRUE;
	}
	else joypresent_ = GL_FALSE;

	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

GLvoid Camera::UpdateShaders(const ShaderProgram& program)
{
	program.Bind();
	UpdateViewMatrixUniform(program.ViewUniformLocation());
	UpdateProjectionMatrixUniform(program.ProjectionUniformLocation());
	UpdatePositionUniform(program.ViewPositionUniformLocation());
	program.Unbind();
}

GLvoid Camera::DoMovement()
{
	UpdateTime();
	GLfloat adjusted_speed = movement_speed_ * dt_;

	if (window_->KeyPressed(GLFW_KEY_W)) { position_ += adjusted_speed * front_; }
	if (window_->KeyPressed(GLFW_KEY_S)) { position_ -= adjusted_speed * front_; }
	if (window_->KeyPressed(GLFW_KEY_A)) { position_ -= glm::normalize(glm::cross(front_, up_)) * adjusted_speed; }
	if (window_->KeyPressed(GLFW_KEY_D)) { position_ += glm::normalize(glm::cross(front_, up_)) * adjusted_speed; }

	// TODO limit the speed if you move Forward, when the Front is towards Up, and Up at the same time
	if (window_->KeyPressed(GLFW_KEY_LEFT_SHIFT	 )) { position_ += up_ * adjusted_speed; }
	if (window_->KeyPressed(GLFW_KEY_LEFT_CONTROL)) { position_ -= up_ * adjusted_speed; }

	/** Process Joystick Inputs **/
	if (joypresent_) 
	{
		// Left Analogue x
		position_ += joyaxes_.at(0) * glm::normalize(glm::cross(front_, up_)) * adjusted_speed;
		// Left Analogue y
		position_ += joyaxes_.at(1) * adjusted_speed * front_;
		// Left Trigger
		position_ -= joyaxes_.at(4) * up_ * adjusted_speed;
		// Right Trigger
		position_ += joyaxes_.at(5) * up_ * adjusted_speed;
	}
}

GLvoid Camera::DoLooking()
{
	UpdateXY();
	yaw_	+= xoffset_ * look_sensitivity_;
	pitch_	+= yoffset_ * look_sensitivity_;

	if (pitch_ > 89.0f )	{ pitch_ =  89.0f; }
	if (pitch_ < -89.0f)	{ pitch_ = -89.0f; }

	front_ = glm::normalize(
		glm::vec3(
			glm::cos(glm::radians(yaw_)) * glm::cos(glm::radians(pitch_)),
			glm::sin(glm::radians(pitch_)),
			glm::sin(glm::radians(yaw_)) * glm::cos(glm::radians(pitch_))
		)
	);
}

GLvoid Camera::UpdateViewMatrix()
{
	DoMovement();
	DoLooking();
	view_mat_ = glm::lookAt(position_, position_ + front_, up_);

	
}

GLvoid Camera::UpdateProjectionMatrix()
{
	GLint width = (*window_).FramebufferWidth();
	GLint height = (*window_).FramebufferHeight();
	projection_mat_ = glm::perspective(
		glm::radians(59.f),
		((GLfloat)width / (GLfloat)height),
		0.5f, 10000.f
	);

	glViewport(0, 0, (*window_).FramebufferWidth(), (*window_).FramebufferHeight());
}

GLvoid Camera::UpdateViewMatrixUniform(GLuint  view_uniform_location) const
{
	glUniformMatrix4fv(view_uniform_location, 1, GL_FALSE, glm::value_ptr(view_mat_));
}

GLvoid Camera:: UpdateProjectionMatrixUniform(GLuint  projection_uniform_location) const
{
	glUniformMatrix4fv(projection_uniform_location, 1, GL_FALSE, glm::value_ptr(projection_mat_));
}

GLvoid Camera::UpdatePositionUniform(GLuint  view_position_uniform) const
{
	glUniform3f(view_position_uniform, position_.x, position_.y, position_.z);
}

GLvoid Camera::UpdateTime()
{
	GLfloat now = glfwGetTime();
	dt_ = now - t_old_;
	t_old_ = now;
}

GLvoid Camera::UpdateXY()
{
	if (first_mouse_movement_ && (*window_).MouseMoved())
	{
		(*window_).GetMousePos(&lastx_, &lasty_);
		first_mouse_movement_ = GL_FALSE;
	}
	GLfloat x, y;
	(*window_).GetMousePos(&x, &y);

	xoffset_ = x - lastx_;
	yoffset_ = lasty_ - y;

	/** Process Joystick Inputs **/
	if (joypresent_)
	{
		// Right Analogue x
		xoffset_ += joyaxes_.at(2) * 50.f;
		// Right Analogue y
		yoffset_ += joyaxes_.at(3) * 50.f;
	}

	/*std::cout << "(" << x << ", " << y << ") "
		<< " || (" << xoffset_ << ", " << yoffset_ << ")"
		<< std::endl;*/

	lastx_ = x;
	lasty_ = y;	
}
