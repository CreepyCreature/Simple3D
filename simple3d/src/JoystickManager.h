#pragma once

#include <GLFW/glfw3.h>

#include <iostream>

#include <vector>

class JoystickManager
{
public:
	// Wrapper enum for the GLFW defines
	enum 
	{
		JOYSTICK_1		=	GLFW_JOYSTICK_1		,
		JOYSTICK_2		=	GLFW_JOYSTICK_2		,
		JOYSTICK_3		=	GLFW_JOYSTICK_3		,
		JOYSTICK_4		=	GLFW_JOYSTICK_4		,
		JOYSTICK_5		=	GLFW_JOYSTICK_5		,
		JOYSTICK_6		=	GLFW_JOYSTICK_6		,
		JOYSTICK_7		=	GLFW_JOYSTICK_7		,
		JOYSTICK_8		=	GLFW_JOYSTICK_8		,
		JOYSTICK_9		=	GLFW_JOYSTICK_9		,
		JOYSTICK_10		=	GLFW_JOYSTICK_10	,
		JOYSTICK_11		=	GLFW_JOYSTICK_11	,
		JOYSTICK_12		=	GLFW_JOYSTICK_12	,
		JOYSTICK_13		=	GLFW_JOYSTICK_13	,
		JOYSTICK_14		=	GLFW_JOYSTICK_14	,
		JOYSTICK_15		=	GLFW_JOYSTICK_15	,
		JOYSTICK_16		=	GLFW_JOYSTICK_16	,
		JOYSTICK_LAST	=	GLFW_JOYSTICK_LAST	,

		MAX_JOYSTICKS	=	GLFW_JOYSTICK_LAST + 1
	};

private:
	bool joysticks_[MAX_JOYSTICKS];

public:

	static JoystickManager* Instance();

	bool JoystickPresent(int joy) const;
	std::vector<float> GetJoystickAxes(int joy);
	std::vector<char> GetJoystickButtons(int joy);


private:
	JoystickManager();
	~JoystickManager();
	JoystickManager(const JoystickManager&) = delete;
	void operator=(const JoystickManager&) = delete;

	static friend void joystick_callback(int joy, int event);
	void joystick_callback_implementation(int joy, int event);

};


// Xbox 360 Controller bindings (achieved empirically)
#define X360_CONTROLLER_A			0
#define X360_CONTROLLER_B			1
#define X360_CONTROLLER_X			2
#define X360_CONTROLLER_Y			3
#define X360_CONTROLLER_LB			4
#define X360_CONTROLLER_RB			5
#define X360_CONTROLLER_BACK		6
#define X360_CONTROLLER_START		7
#define X360_CONTROLLER_LEFT_STICK	8
#define X360_CONTROLLER_RIGHT_STICK	9
#define X360_CONTROLLER_UP			10
#define X360_CONTROLLER_RIGHT 		11
#define X360_CONTROLLER_DOWN 		12
#define X360_CONTROLLER_LEFT 		13

// Wrapper defines
//#define JOYSTICK_1		GLFW_JOYSTICK_1
//#define JOYSTICK_2		GLFW_JOYSTICK_2
//#define JOYSTICK_3		GLFW_JOYSTICK_3
//#define JOYSTICK_4		GLFW_JOYSTICK_4
//#define JOYSTICK_5		GLFW_JOYSTICK_5
//#define JOYSTICK_6		GLFW_JOYSTICK_6
//#define JOYSTICK_7		GLFW_JOYSTICK_7
//#define JOYSTICK_8		GLFW_JOYSTICK_8
//#define JOYSTICK_9		GLFW_JOYSTICK_9
//#define JOYSTICK_10		GLFW_JOYSTICK_10
//#define JOYSTICK_11		GLFW_JOYSTICK_11
//#define JOYSTICK_12		GLFW_JOYSTICK_12
//#define JOYSTICK_13		GLFW_JOYSTICK_13
//#define JOYSTICK_14		GLFW_JOYSTICK_14
//#define JOYSTICK_15		GLFW_JOYSTICK_15
//#define JOYSTICK_16		GLFW_JOYSTICK_16
//#define JOYSTICK_LAST	GLFW_JOYSTICK_LAST
//
//#define MAX_JOYSTICS	JOYSTICK_LAST + 1
