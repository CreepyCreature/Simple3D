#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif // !GLEW_STATIC
#include <GL/glew.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

#include "Material.h"
#include "Mesh.h"
#include "../ShaderProgram.h"

class Model
{
private:
	std::vector<Mesh> meshes_;
	std::string directory_;
	std::vector<Texture> textures_loaded_;

	glm::mat4 model_matrix_;
	GLuint model_uniform_;

	glm::mat4 translation_matrix_;
	glm::mat4 rotation_matrix_;
	glm::mat4 scaling_matrix_;

	static const char* default_texture_path_;
public:
	Model(const GLchar* path);
	Model(const std::vector<Mesh>& meshes);
	Model() = delete; // TODO
	~Model();

	GLvoid Draw(const ShaderProgram& program);

	/* Translation */	
	GLvoid Translate(GLfloat x, GLfloat y, GLfloat z);
	GLvoid Translate(const glm::vec3& position);
	GLvoid TranslateBy(GLfloat x, GLfloat y, GLfloat z);
	GLvoid TranslateBy(const glm::vec3& position);
	/* Rotation */
	GLvoid Rotate(GLfloat degrees, GLfloat x, GLfloat y, GLfloat z);
	GLvoid Rotate(GLfloat degrees, const glm::vec3& axis);
	GLvoid RotateBy(GLfloat degrees, GLfloat x, GLfloat y, GLfloat z);
	GLvoid RotateBy(GLfloat degrees, const glm::vec3& axis);
	/* Scaling */
	GLvoid Scale(GLfloat x, GLfloat y, GLfloat z);
	GLvoid Scale(const glm::vec3& position);
	GLvoid ScaleBy(GLfloat x, GLfloat y, GLfloat z);
	GLvoid ScaleBy(const glm::vec3& position);
private:
	GLvoid Init();
	GLvoid UpdateModelMatrix(const ShaderProgram& program);

	GLvoid LoadModel(const std::string& path);
	GLvoid ProcessNode(aiNode* node, const aiScene* const scene);
	Mesh ProcessMesh(const aiMesh* mesh, const aiScene* const scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* material,
		aiTextureType type /*, string typename*/);
	GLfloat LoadMaterialShininess(aiMaterial* material,
		aiTextureType type);

	static GLuint TextureFromFile(const GLchar* path, const std::string& directory);
	static GLuint TextureDefault(); // White 1x1 texture
};

