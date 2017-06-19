#include "JoystickManager.h"

JoystickManager::JoystickManager()
{
	for (int i = 0; i < MAX_JOYSTICKS; ++i)
	{
		joysticks_[i] = (glfwJoystickPresent(i) == GLFW_TRUE);
	}

	glfwSetJoystickCallback(joystick_callback);
}

JoystickManager::~JoystickManager()
{
}

JoystickManager* JoystickManager::Instance()
{
	static JoystickManager instance;
	return &instance;
}

bool JoystickManager::JoystickPresent(int joy) const
{
	if (joy >= MAX_JOYSTICKS || joy < 0) { return false; }
	return joysticks_[joy];
}

std::vector<float> JoystickManager::GetJoystickAxes(int joy)
{
	// TODO: insert return statement here
	if (joy > JOYSTICK_LAST || joy < 0) { return std::vector<float>(); }

	int count;
	const float* axes = glfwGetJoystickAxes(joy, &count);
	std::vector<float> axes_vect(axes, axes + count);

	return axes_vect;
}

std::vector<char> JoystickManager::GetJoystickButtons(int joy)
{
	if (joy > JOYSTICK_LAST || joy < 0) { return std::vector<char>(); }

	int count;
	const unsigned char* buttons = glfwGetJoystickButtons(joy, &count);
	std::vector<char> buttons_vect(buttons, buttons + count);

	return buttons_vect;
}

void joystick_callback(int joy, int event)
{
	JoystickManager::Instance()->joystick_callback_implementation(joy, event);
}
void JoystickManager::joystick_callback_implementation(int joy, int event)
{
	if (joy >= MAX_JOYSTICKS)
	{
		std::cout << "There's something wrong with "
			<< "the joystick callback!" << std::endl;
		return;
	}

	joysticks_[joy] = (event == GLFW_CONNECTED);

	if (event == GLFW_CONNECTED)
	{
		const char* name = glfwGetJoystickName(joy);
		std::cout << name << " (#" << joy << ")"
			<< " connected!" << std::endl;
	}
	else if (event == GLFW_DISCONNECTED)
	{
		std::cout << "Joystick #" << joy
			<< " disconnected!" << std::endl;
	}
}