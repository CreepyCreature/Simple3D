#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif // !GLEW_STATIC

#include <GL\glew.h>

#include <string>
#include <vector>

class ShaderProgram
{
public:
	enum class ShaderStage
	{
		VERTEX_SHADER	= GL_VERTEX_SHADER	,
		GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
		FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
		COMPUTE_SHADER	= GL_COMPUTE_SHADER
	};
private:
	GLuint program_;	
	std::vector<GLuint> attached_shaders_;

	// Usual Uniform Locations
	GLuint model_uniform_			;
	GLuint view_uniform_			;
	GLuint projection_uniform_		;
	GLuint view_position_uniform_	;
public:
	ShaderProgram();
	~ShaderProgram();

	// Passing by value is disallowed to avoid calling the destructor.
	// Passing by value copies the ID of the real object, and when the 
	// destructor is called it destroys the real object's GL stuff.
	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;

	// Loads the shader source code from [filepath], creates a GL Shader 
	// Object of the [stage] shader type and attaches it to the program.
	GLboolean LoadFromFile(const GLchar* filepath, ShaderStage stage);
	// Creates and compiles a GL Shader Object of the [stage] shader type
	// using the [sh_source_str] source code and attaches it to the program.
	GLboolean LoadShader(const std::string& sh_source_str, ShaderStage stage);

	// Links the currently attached shaders to the program.
	// If the link fails it deletes the program and all attached
	// shaders.
	GLboolean Link();

	// TODO: Check the status and do not use the program unless it is successfully linked.
	GLvoid Bind() const;
	GLvoid Unbind() const;

	GLuint ID() const;

	GLuint ModelUniformLocation()			const;
	GLuint ViewUniformLocation()			const;
	GLuint ProjectionUniformLocation()		const;
	GLuint ViewPositionUniformLocation()	const;

	GLvoid SetUniform1i(const GLchar*, GLint);
private:
	GLuint GetModelUniformLocation();
	GLuint GetViewUniformLocation();
	GLuint GetProjectionUniformLocation();
	GLuint GetViewPositionUniformLocation()	;
};

