#pragma once

#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

#include <list>

class Renderer
{
private:
public:
	Renderer();
	~Renderer();

	bool Init();
	
};

