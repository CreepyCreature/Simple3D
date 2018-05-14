#include "Renderer.h"

#include <iostream>

Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

bool Renderer::Init()
{
	if (glfwGetCurrentContext() == NULL)
	{
		printf("%s\n", "RENDERER: No context current available!");
		return false;
	}
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) { 
		printf( "%s\n", glewGetErrorString(glGetError()) );
		return false; 
	}
	printf("%s\n", glGetString(GL_VERSION));

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_FRAMEBUFFER_SRGB);

	return true;
}
