#include "Window.h"


Window::Window(GLuint width, GLuint height,
	const char* title,
	WINDOW_MODE window_mode,
	CURSOR_MODE cursor_mode
)
{
	width_ = width;
	height_ = height;
	title_ = title;
	window_mode_ = window_mode;
	cursor_mode_ = cursor_mode;

	if ( !Init() ) { glfwTerminate(); }
}

bool Window::Init()
{
	// Initialize GLFW
	if (glfwInit() != GLFW_TRUE)
	{
		std::cout << "Could not initialize GLFW!" << std::endl;
		return false;
	}

	// Set GLFW Window hints
	glfwWindowHint(GLFW_SAMPLES, 8);
	//glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

	// Set the window mode accordingly
	GLFWmonitor* monitor = NULL;
	if (window_mode_ == WINDOW_MODE::BORDERLESS)
	{
		monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);

		glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);

		width_ = vidmode->width;
		height_ = vidmode->height;
	}
	else if (window_mode_ == WINDOW_MODE::FULLSCREEN)
	{
		monitor = glfwGetPrimaryMonitor();
	}
	// Create the window
	window_ = glfwCreateWindow(width_, height_, title_, monitor, NULL);
	if (!window_)
	{
		std::cout << "Could not create window!" << std::endl;
		glfwTerminate();
		return false;
	}	
	// Assign the window to the current context
	glfwMakeContextCurrent(window_);

	// Make sure the framebuffer sizes are assigned
	glfwGetFramebufferSize(window_, &framebuffer_width_, &framebuffer_height_);

	// Map the GLFW window pointer to our Window object
	// (this is important for implementing the callbacks)
	glfwSetWindowUserPointer(window_, this);

	// Set the cursor mode
	glfwSetInputMode(window_, GLFW_CURSOR, (GLint)cursor_mode_);

	// Set the key(board) callback
	glfwSetKeyCallback(window_, key_callback);
	// Set the mouse position (cursor) callback
	glfwSetCursorPosCallback(window_, cursor_position_callback);
	// Set the mouse button callback
	glfwSetMouseButtonCallback(window_, mouse_button_callback);
	// Set the window callbacks
	glfwSetWindowSizeCallback(window_, window_size_callback);
	glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);

	// Initialize the input arrays
	for (int i = 0; i < MAX_KEYS; ++i) {
		keystates_[i] = INPUT_UNREGISTERED;
	}
	mouse_istate_ = INPUT_UNREGISTERED;
	for (int i = 0; i < MAX_MOUSE_BUTTONS; ++i) {
		mbstates_[i] = INPUT_UNREGISTERED;
	}	

	return true;
}

Window::~Window()
{
	glfwDestroyWindow(window_);
	glfwTerminate();
}

void Window::Update()
{
	glfwSwapBuffers(window_);
	glfwPollEvents();

	glClearColor(0.2f, 0.4f, 0.85f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(window_);
}

void Window::MakeContextCurrent()
{
	if (window_)	glfwMakeContextCurrent(window_);
}

bool Window::KeyPressed(unsigned int key)
{
	if (key > MAX_KEYS) { return false; }

	// return keys_[key];	
	return (keystates_[key] != INPUT_UNREGISTERED);
}

bool Window::KeyDown(unsigned int key)
{
	if (key > MAX_KEYS) { return false; }

	bool retval = (keystates_[key] == INPUT_REGISTERED);
	if (retval) 
	{
		keystates_[key] = INPUT_USED;
		// std::cout << "Keyused\n";
	}
	return retval;
}

bool Window::MouseButtonPressed(unsigned int button)
{
	if (button > MAX_MOUSE_BUTTONS) { return false; }

	// return mouse_buttons_[button];
	return (mbstates_[button] != INPUT_UNREGISTERED);
}

bool Window::MouseButtonDown(unsigned int button)
{
	if (button > MAX_MOUSE_BUTTONS) { return false; }

	bool retval = (mbstates_[button] == INPUT_REGISTERED);
	if (retval)
	{
		mbstates_[button] = INPUT_USED;
		// std::cout << "Keyused\n";
	}
	return retval;
}

void Window::GetMousePos(double* xpos, double* ypos)
{
	*xpos = mousex_;
	*ypos = mousey_;
}

void Window::GetMousePos(float* xpos, float* ypos)
{
	*xpos = mousex_;
	*ypos = mousey_;
}

bool Window::MouseMoved()
{
	if (mouse_istate_ == INPUT_REGISTERED)
	{
		mouse_istate_ = INPUT_USED;
		return true;
	}
	return false;
}

// Non-essential components

void Window::SetCursorMode(Window::CURSOR_MODE cursor_mode)
{
	cursor_mode_ = cursor_mode;
	glfwSetInputMode(window_, GLFW_CURSOR, (GLint)cursor_mode_);
}

int Window::Width() const
{
	return width_;
}

int Window::Height() const
{
	return height_;
}

int Window::FramebufferWidth() const
{
	return framebuffer_width_;
}

int Window::FramebufferHeight() const
{
	return framebuffer_height_;
}

Window::WINDOW_MODE Window::WindowMode() const
{
	return window_mode_;
}

void Window::MakeFullscreen()
{
	if (window_mode_ == WINDOW_MODE::FULLSCREEN) return;

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);

	glfwSetWindowMonitor(window_, monitor, 0, 0, vidmode->width, vidmode->height, vidmode->refreshRate);
	window_mode_ = WINDOW_MODE::FULLSCREEN;	
}

void Window::MakeWindowed(int xpos, int ypos, int width, int height)
{
	if (window_mode_ == WINDOW_MODE::WINDOWED) return;

	if (xpos < 0 || ypos < 0 || width < 0 || height < 0)
	{
		GLFWmonitor* monitor = glfwGetWindowMonitor(window_);
		const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);

		width = vidmode->width / 2;
		height = vidmode->height / 2;
		xpos = width / 2;
		ypos = height / 2;
	}
	glfwSetWindowMonitor(window_, NULL, xpos, ypos, width, height, NULL);
	window_mode_ = WINDOW_MODE::WINDOWED;
}

// Input callbacks

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Window* winobj = (Window*)glfwGetWindowUserPointer(window);
	if (!winobj) { return; }

	if (winobj->keystates_[key] == Window::INPUT_UNREGISTERED
		&& action != GLFW_RELEASE)
	{
		winobj->keystates_[key] = Window::INPUT_REGISTERED;
		// std::cout << "Keydown\n";
	}
	else if (action == GLFW_RELEASE)
	{
		winobj->keystates_[key] = Window::INPUT_UNREGISTERED;
		// std::cout << "Keyrelease\n";
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Window* winobj = (Window*)glfwGetWindowUserPointer(window);
	if (!winobj) { return; }

	if (winobj->mbstates_[button] == Window::INPUT_UNREGISTERED
		&& action != GLFW_RELEASE)
	{
		winobj->mbstates_[button] = Window::INPUT_REGISTERED;
		// std::cout << "Keydown\n";
	}
	else if (action == GLFW_RELEASE)
	{
		winobj->mbstates_[button] = Window::INPUT_UNREGISTERED;
		// std::cout << "Keyrelease\n";
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Window* winobj = (Window*)glfwGetWindowUserPointer(window);
	if (!winobj) { return; }

	double oldx = winobj->mousex_;
	double oldy = winobj->mousey_;
	winobj->mousex_ = xpos;
	winobj->mousey_ = ypos;

	// I know I could've left out the first part of the following
	// if-statement but I feel more comfy having it anyway.
	if (winobj->mouse_istate_ == Window::INPUT_UNREGISTERED
		&& (oldx != xpos || oldy != ypos))
	{
		winobj->mouse_istate_ = Window::INPUT_REGISTERED;
	}
	else 
	{
		winobj->mouse_istate_ = Window::INPUT_UNREGISTERED;
	}
}

void joystick_callback(int joy, int event)
{
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

void window_size_callback(GLFWwindow * window, int width, int height)
{
	Window* winobj = (Window*)glfwGetWindowUserPointer(window);
	if (!winobj) { return; }

	winobj->width_ = width;
	winobj->height_ = height;
}

void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	Window* winobj = (Window*)glfwGetWindowUserPointer(window);
	if (!winobj) { return; }

	winobj->framebuffer_width_ = width;
	winobj->framebuffer_height_ = height;
}
