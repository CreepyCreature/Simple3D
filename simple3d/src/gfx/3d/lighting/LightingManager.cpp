#include "LightingManager.h"



LightingManager::LightingManager()
{
}


LightingManager::~LightingManager()
{
}

GLvoid LightingManager::UpdateShaders(const ShaderProgram& program)
{
	// Update Directional Light Data
	for (GLuint i = 0; i < directional_lights_.size(); ++i)
	{
		(*directional_lights_.at(i)).UpdateShaders(program, i);
	}
	program.Bind();
	glUniform1i(
		glGetUniformLocation(program.ID(), "u_nrdirlights"),
		directional_lights_.size()
	);
	program.Unbind();

	// Update Point Light Data
	for (GLuint i = 0; i < point_lights_.size(); ++i)
	{
		(*point_lights_.at(i)).UpdateShaders(program, i);
	}
	program.Bind();
	glUniform1i(
		glGetUniformLocation(program.ID(), "u_nrpointlights"),
		point_lights_.size()
	);
	program.Unbind();

	// Update Spot Light Data
	for (GLuint i = 0; i < spot_lights_.size(); ++i)
	{
		(*spot_lights_.at(i)).UpdateShaders(program, i);
	}
	program.Bind();
	glUniform1i(
		glGetUniformLocation(program.ID(), "u_nrspotlights"),
		spot_lights_.size()
	);
	program.Unbind();
}

GLvoid LightingManager::AddLight(const DirectionalLight* directional_light)
{
	directional_lights_.push_back(directional_light);
}

GLvoid LightingManager::AddLight(const PointLight* point_light)
{
	point_lights_.push_back(point_light);
}

GLvoid LightingManager::AddLight(const SpotLight * spot_light)
{
	spot_lights_.push_back(spot_light);
}
