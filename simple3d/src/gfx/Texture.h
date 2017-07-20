#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif // !GLEW_STATIC
#include <GL/glew.h>

#include <assimp/material.h>

#include <string>

enum class TextureType
{
	NONE			= aiTextureType::aiTextureType_NONE			, 
	DIFFUSE_MAP		= aiTextureType::aiTextureType_DIFFUSE		,
	SPECULAR_MAP	= aiTextureType::aiTextureType_SPECULAR
};

struct Texture
{
	GLuint id;
	TextureType type;
	aiString path;

	bool placeholder;
};