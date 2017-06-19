#include "Model.h"

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif // !GLEW_STATIC
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

// #define DEBUG // <- Uncomment this to print out debug info

Model::Model(const GLchar* path)
{
	LoadModel(path);
	Init();
}

Model::Model(const std::vector<Mesh>& meshes) : meshes_(meshes)
{
	Init();
}

Model::~Model()
{
}

GLvoid Model::Init()
{
	model_matrix_ = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
	//model_uniform_ = glGetUniformLocation()

	translation_matrix_		= glm::mat4(1.0f);
	rotation_matrix_		= glm::mat4(1.0f);
	scaling_matrix_			= glm::mat4(1.0f);
}

GLvoid Model::Draw(const ShaderProgram & program)
{
	UpdateModelMatrix(program);
	for (const Mesh& mesh : meshes_)
	{
		mesh.Draw(program);
	}
}

GLvoid Model::UpdateModelMatrix(const ShaderProgram& program)
{
	model_matrix_ = translation_matrix_ * rotation_matrix_ * scaling_matrix_;

	program.Bind();
	glUniformMatrix4fv(
		glGetUniformLocation(program.ID(), "u_model"),
		1, GL_FALSE, glm::value_ptr(model_matrix_)
	);
	program.Unbind();
}

/* Translation */

GLvoid Model::Translate(GLfloat x, GLfloat y, GLfloat z)
{
	translation_matrix_ = glm::translate(glm::vec3(x, y, z));
}

GLvoid Model::Translate(const glm::vec3& position)
{
	translation_matrix_ = glm::translate(glm::vec3(position));
}

GLvoid Model::TranslateBy(GLfloat x, GLfloat y, GLfloat z)
{
	translation_matrix_ = glm::translate(translation_matrix_, glm::vec3(x, y, z));
}

GLvoid Model::TranslateBy(const glm::vec3& position)
{
	translation_matrix_ = glm::translate(translation_matrix_, glm::vec3(position));
}

/* Rotation */

GLvoid Model::Rotate(GLfloat degrees, GLfloat x, GLfloat y, GLfloat z)
{
	rotation_matrix_ = glm::rotate(glm::radians(degrees), glm::vec3(x, y, z));
}

GLvoid Model::Rotate(GLfloat degrees, const glm::vec3& axis)
{
	rotation_matrix_ = glm::rotate(glm::radians(degrees), axis);
}

GLvoid Model::RotateBy(GLfloat degrees, GLfloat x, GLfloat y, GLfloat z)
{
	rotation_matrix_ = glm::rotate(rotation_matrix_, glm::radians(degrees), glm::vec3(x, y, z));
}

GLvoid Model::RotateBy(GLfloat degrees, const glm::vec3& axis)
{
	rotation_matrix_ = glm::rotate(rotation_matrix_, glm::radians(degrees), axis);
}

/* Scale */

GLvoid Model::Scale(GLfloat x, GLfloat y, GLfloat z)
{
	scaling_matrix_ = glm::scale(glm::vec3(x, y, z));
}

GLvoid Model::Scale(const glm::vec3& axes)
{
	scaling_matrix_ = glm::scale(axes);
}

GLvoid Model::ScaleBy(GLfloat x, GLfloat y, GLfloat z)
{
	scaling_matrix_ = glm::scale(scaling_matrix_, glm::vec3(x, y, z));
}

GLvoid Model::ScaleBy(const glm::vec3& axes)
{
	scaling_matrix_ = glm::scale(scaling_matrix_, axes);
}

GLvoid Model::LoadModel(const std::string & path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	if (!scene || !scene->mRootNode || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE)
	{
		std::cout << "Model: error loading model from file!" << std::endl;
		std::cout << "ASSIMP: " << importer.GetErrorString() << std::endl;
	}

	std::size_t lastfolder = path.find_last_of('/');
	if (lastfolder == std::string::npos)
	{
		lastfolder = path.find_last_of('\\');
	}
	directory_ = path.substr(0, lastfolder);

	ProcessNode(scene->mRootNode, scene);
}

GLvoid Model::ProcessNode(aiNode * node, const aiScene * const scene)
{
	// Process the meshes in this node
	for (GLuint i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[ node->mMeshes[i] ];
		meshes_.push_back(ProcessMesh(mesh, scene));
	}

	// Process any child node
	for (GLuint i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(const aiMesh * mesh, const aiScene * const scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	GLfloat shininess;

	#ifdef DEBUG
	std::cout << "Processing Mesh" << std::endl;
	#endif // DEBUG

	// Vertex Data
	for (GLuint i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		// Vertex Position
		if (mesh->HasPositions())
		{
			vertex.position.x = mesh->mVertices[i].x;
			vertex.position.y = mesh->mVertices[i].y;
			vertex.position.z = mesh->mVertices[i].z;
		}
		// Vertex Normal
		if (mesh->HasNormals()) 
		{
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;
		}
		// Vertex UV (Texture Coordinates)
		if (mesh->mTextureCoords[0])
		{
			vertex.tex_coords.x = mesh->mTextureCoords[0][i].x;
			vertex.tex_coords.y = mesh->mTextureCoords[0][i].y;
		}
		else 
		{
			vertex.tex_coords.x = 0.f;
			vertex.tex_coords.y = 0.f;
		}
		// TODO Vertex Color 
		

		vertices.push_back(vertex);
	}
	// Indices
	if (mesh->HasFaces())
	{
		for (GLuint i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];
			for (GLuint j = 0; j < face.mNumIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}
	}
	// Material Data
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diff_maps = LoadMaterialTextures(material, aiTextureType_DIFFUSE);
		textures.insert(textures.end(), diff_maps.begin(), diff_maps.end());

		std::vector<Texture> spec_maps = LoadMaterialTextures(material, aiTextureType_SPECULAR);
		textures.insert(textures.end(), spec_maps.begin(), spec_maps.end());

		shininess = LoadMaterialShininess(material, aiTextureType_SHININESS);
	}

	return Mesh(vertices, indices, textures, shininess);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial * material, aiTextureType type)
{
	std::vector<Texture> textures;

	for (GLuint i = 0; i < material->GetTextureCount(type); ++i)
	{
		aiString str;
		material->GetTexture(type, i, &str);
		GLboolean texture_loaded = GL_FALSE;
		for (GLuint j = 0; j < textures_loaded_.size(); ++j)
		{
			if (textures_loaded_[j].path == str)
			{
				textures.push_back(textures_loaded_[j]);
				texture_loaded = GL_TRUE;
				break;
			}
		}
		if (!texture_loaded)
		{
			Texture texture;
			texture.id		= TextureFromFile(str.C_Str(), directory_);
			texture.type	= (TextureType)type;
			texture.path	= str;

			if (texture.type == TextureType::SPECULAR_MAP)
			{
				std::cout <<  texture.id 	<< " ";
				std::cout <<  (int)texture.type	<< " ";
				std::cout <<  texture.path.C_Str()	<< " ";
				std::cout << std::endl;
			}

			textures.push_back(texture);
			textures_loaded_.push_back(texture);
		}
		
		#ifdef DEBUG
		std::cout << "Loaded texture #" << i << std::endl;
		#endif // DEBUG
	}

	return textures;
}

GLfloat Model::LoadMaterialShininess(aiMaterial * material, aiTextureType type)
{
	GLfloat shiny = 1.0f;
	material->Get(AI_MATKEY_SHININESS, shiny);

	return shiny;
}

GLuint Model::TextureFromFile(const GLchar * path, const std::string & directory)
{
	std::string filestr(path);
	filestr = directory + '/' + filestr;

	std::cout << "Loading Texture from: " << filestr;

	// Load the image bytes
	GLint width, height;
	GLubyte* image = stbi_load(filestr.c_str(), &width, &height, 0, STBI_rgb);

	std::cout << " (" << width << ", " << height << ")" << std::endl;

	// Generate and bind the texture
	GLuint texid;
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	// Assign the bytes from the image to the texture and generate mipmaps for it
	glTexImage2D(GL_TEXTURE_2D, NULL, GL_RGB, width, height, NULL, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Set some useful texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Enable Anisotropic Filtering if it is supported
	#ifdef GL_EXT_texture_filter_anisotropic
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 32);
	#endif // GL_EXT_texture_filter_anisotropic
	#ifndef GL_EXT_texture_filter_anisotropic
	std::cout << "Anisotropic filtering not supported!" << std::endl;
	#endif // !GL_EXT_texture_filter_anisotropic

	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return texid;
}