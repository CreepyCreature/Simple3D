#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

// TODO:
/*
 *	~ Make up my mind on Enums vs Defined Macros
 */

// Notes:
/*
 * + Moved the joystick callback and handling 
 *	 to the Input (singleton) class.
 * 
 */

#define MAX_KEYS			1024
#define MAX_MOUSE_BUTTONS	32

class Window
{
public:
	enum class WINDOW_MODE
	{
		WINDOWED, BORDERLESS, FULLSCREEN
	};
	enum class CURSOR_MODE
	{
		CURSOR_NORMAL	= GLFW_CURSOR_NORMAL	, 
		CURSOR_HIDDEN	= GLFW_CURSOR_HIDDEN	,
		CURSOR_DISABLED	= GLFW_CURSOR_DISABLED
	};

private:
	GLFWwindow* window_;
	GLuint width_, height_;
	GLint framebuffer_width_, framebuffer_height_;
	const GLchar* title_;
	WINDOW_MODE window_mode_;
	CURSOR_MODE cursor_mode_;

	// Input handling variables
	enum INPUTSTATE
	{
		INPUT_UNREGISTERED, INPUT_REGISTERED, INPUT_USED 
	};

	INPUTSTATE keystates_[MAX_KEYS];
	INPUTSTATE mbstates_[MAX_MOUSE_BUTTONS];
	double mousex_, mousey_;
	INPUTSTATE mouse_istate_;

public:
	Window(GLuint width = 800, GLuint height = 600, 
		const char* title = "Sample Text",
		WINDOW_MODE window_mode = WINDOW_MODE::WINDOWED,
		CURSOR_MODE cursor_mode = CURSOR_MODE::CURSOR_NORMAL
	);
	~Window();

	void Update();
	bool ShouldClose();	
	void MakeContextCurrent();

	bool KeyPressed(unsigned int key);
	bool KeyDown(unsigned int key);
	bool MouseButtonPressed(unsigned int button);
	bool MouseButtonDown(unsigned int button);
	void GetMousePos(double* xpos, double* ypos);
	void GetMousePos(float* xpos, float* ypos);
	void GetMouseOffset(float* xoffset, float* yoffset);
	bool MouseMoved();

	// Non-essential components
	void SetCursorMode(CURSOR_MODE cursor_mode);
	int Width() const;
	int Height() const;
	int FramebufferWidth() const;
	int FramebufferHeight() const;
	WINDOW_MODE WindowMode() const;
	void MakeFullscreen();
	void MakeWindowed(int xpos = -1, int ypos = -1, int width = -1, int height = -1);

private:
	bool Init();

	// Keyboard callback
	static friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	// Mouse callbacks (position and buttons)
	static friend void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	// Joystick callbacks
	static friend void joystick_callback(int joy, int event);

	// Window callbacks
	static friend void window_size_callback(GLFWwindow* window, int width, int height);
	static friend void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};