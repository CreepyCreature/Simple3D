#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif // !GLEW_STATIC
#include <gl/glew.h>

#include <vector>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class LightingManager
{
private:
	std::vector<const DirectionalLight*> directional_lights_;
	std::vector<const PointLight*> point_lights_;
	std::vector<const SpotLight*> spot_lights_;
public:
	LightingManager();
	~LightingManager();

	GLvoid UpdateShaders(const ShaderProgram& program);

	GLvoid AddDirectionalLight(const DirectionalLight* directional_light);
	GLvoid AddPointLight(const PointLight* point_light);
	GLvoid AddLight(const SpotLight* spot_light);
	GLvoid AddLight(const PointLight* spot_light);
	GLvoid AddLight(const DirectionalLight* spot_light);
};

