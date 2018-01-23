#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif // !GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>

#include <vector>
#include <unordered_map>

#include "Material.h"
#include "Vertex.h"
#include "../Texture.h"
#include "../ShaderProgram.h"

class Mesh
{
private:
	// Vertex Data
	std::vector<Vertex> vertices_;	
	std::vector<GLuint> indices_;
	// Material Data
	std::vector<Texture> textures_;
	Material material_;

	mutable std::unordered_map<std::string, int> uniform_location_cache_;

	// Buffer Objects
	GLuint vao_;
	GLuint vbo_;
	GLuint ebo_;

	GLboolean initialised_ = GL_FALSE;
public:
	Mesh();
	Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures, GLfloat shininess);
	Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures, const Material& material);
	Mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& colors);
	// This has vec3 for colors, alpha is set to 1.0f
	Mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& colors, const std::vector<GLuint>& indices);
	~Mesh();

	void Draw(const ShaderProgram& program) const;

	void Rainbow();
	void MultiRainbow();
private:
	void Setup();
	void InitUniforms();
	void DrawFully(const ShaderProgram&) const;
	void DrawVertices() const;
	void DrawElements() const;
	void UpdateVertexBuffer();
	int GetUniformLocation(unsigned int program_id, const std::string& name) const;
};

