#include "Mesh.h"

#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

#include <iostream>

// #define DEBUG // <- Uncomment this to print out debug info

Mesh::Mesh()
{
	initialised_ = GL_FALSE;
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures, GLfloat shininess) :
	vertices_(vertices), indices_(indices), textures_(textures), specular_shininess_(shininess)
{
	Setup();
}

Mesh::Mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& colors)
{
	if (positions.size() != colors.size())
	{
		return;
	}

	vertices_.resize(positions.size());
	for (GLuint i = 0; i < vertices_.size(); ++i)
	{
		vertices_[i].position = positions[i];
		vertices_[i].color = colors[i];
	}

	Setup();
}

Mesh::Mesh(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& colors, const std::vector<GLuint>& indices) :
	indices_(indices)
{
	if (positions.size() != colors.size())
	{
		return;
	}

	vertices_.resize(positions.size());
	for (GLuint i = 0; i < vertices_.size(); ++i)
	{
		vertices_[i].position = positions[i];
		vertices_[i].color = colors[i];
	}

	Setup();
}

Mesh::~Mesh()
{
}

void Mesh::Setup()
{
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, 
		vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

	if (!indices_.empty())
	{
		glGenBuffers(1, &ebo_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
			indices_.size() * sizeof(GLuint), &indices_[0], GL_STATIC_DRAW);
	}

	// Vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
		(GLvoid*)offsetof(Vertex, position));
	// Vertex normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
		(GLvoid*)offsetof(Vertex, normal));
	// Vertex texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
		(GLvoid*)offsetof(Vertex, tex_coords));
	// Vertex color
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
		(GLvoid*)offsetof(Vertex, color));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	initialised_ = GL_TRUE;
}

void Mesh::Draw(const ShaderProgram& program) const
{
	if (!initialised_) { return; }

	program.Bind();

	if (indices_.empty() && textures_.empty())
	{
		DrawVertices();
	}
	else if (textures_.empty())
	{
		DrawElements();
	}
	else
	{
		DrawFully(program);
	}

	program.Unbind();
}

void Mesh::DrawFully(const ShaderProgram& program) const
{
	for (GLuint i = 0; i < textures_.size(); ++i)
	{
		// Activate the corresponding Texture Unit
		glActiveTexture(GL_TEXTURE0 + i);
		// Bind the corresponding Texture to the Texture Unit
		glBindTexture(GL_TEXTURE_2D, textures_[i].id);

		#ifdef DEBUG
		std::cout << "Texture[" << i << "] :: ID " <<
			textures_[i].id << " :: Type " << (int)textures_[i].type <<
			" :: Path " << textures_[i].path.C_Str() <<
			std::endl;
		#endif // DEBUG

		std::string materialdiff = "material.diffuse";
		std::string materialspec = "material.specular";
		std::string materialshiny = "material.shininess";

		// Get the uniform location for the Sampler
		GLint sampler_uniform = -2;
		if (textures_[i].type == TextureType::DIFFUSE_MAP)
		{
			#ifdef DEBUG
			std::cout << "Applying diffuse map!" << std::endl;
			#endif // DEBUG

			sampler_uniform = glGetUniformLocation(program.ID(), materialdiff.c_str());
		}
		else if (textures_[i].type == TextureType::SPECULAR_MAP)
		{
			#ifdef DEBUG
			std::cout << "Applying specular map!" << std::endl;
			#endif // DEBUG

			sampler_uniform = glGetUniformLocation(program.ID(), materialspec.c_str());
			glUniform1f(glGetUniformLocation(program.ID(), materialshiny.c_str()),
				specular_shininess_);
		}

		#ifdef DEBUG
		std::cout << "Uniform: " << sampler_uniform << std::endl;
		std::cout << "Program ID: " << program.ID() << std::endl;
		#endif // DEBUG

		// Pass the Texture Unit ID to the Sampler
		glUniform1i(sampler_uniform, i);
	}

	// Not exactly necessary but reset the e
	// Active Textur Unit to 0
	glActiveTexture(GL_TEXTURE0);

	// Draw the mesh
	DrawElements();

	// Unbind the Textures from their
	// Texturing Units (just in case)
	for (GLuint i = 0; i < textures_.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::DrawVertices() const
{
	glBindVertexArray(vao_);
	glDrawArrays(GL_TRIANGLES, 0, vertices_.size());
	glBindVertexArray(0);
}

void Mesh::DrawElements() const
{
	glBindVertexArray(vao_);
	glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::UpdateVertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);	
}

// Taste the rainbow
void Mesh::Rainbow()
{
	if (!initialised_) { return; }

	for (Vertex& v : vertices_)
	{
		v.color = glm::vec3(
			(glm::sin(glm::radians(glfwGetTime() * 30) * 2.0f) + 1.f) / 2.f,
			(glm::cos(glm::radians(glfwGetTime() * 30) * 2.2f) + 1.f) / 2.f,
			(glm::sin(glm::radians(glfwGetTime() * 30) * 3.4f) + 1.f) / 2.f
		);
	}
	UpdateVertexBuffer();
}

void Mesh::MultiRainbow()
{
	if (!initialised_) { return; }

	for (GLuint i = 0; i < vertices_.size(); ++i)
	{
		vertices_.at(i).color = glm::vec3(
			(glm::sin(glm::radians(glfwGetTime() * 30) * 2.0f + (i*i)) + 1.f) / 2.f,
			(glm::cos(glm::radians(glfwGetTime() * 30) * 2.2f + (i*i)) + 1.f) / 2.f,
			(glm::sin(glm::radians(glfwGetTime() * 30) * 3.4f + (i*i)) + 1.f) / 2.f
		);
	}
	UpdateVertexBuffer();
}