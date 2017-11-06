#include "ShaderProgram.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

ShaderProgram::ShaderProgram()
{
	program_ = glCreateProgram();	
}

// When passing the Program by value it gets destroyed and deletes
// eveything from the actual Program. Check for this!
ShaderProgram::~ShaderProgram()
{
	std::cout << "DESTROYING SHADER PROGRAM!" << std::endl;
	glDeleteProgram(program_);
	for (GLint i = 0; i < attached_shaders_.size(); ++i)
	{
		glDetachShader(program_, attached_shaders_.at(i));
		glDeleteShader(attached_shaders_.at(i));
	}
}

GLboolean ShaderProgram::LoadFromFile(const GLchar * filepath, ShaderStage stage)
{
	std::string sh_source_str;
	std::ifstream sh_file;
	sh_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try 
	{
		sh_file.open(filepath);
		std::stringstream sh_stream;
		sh_stream << sh_file.rdbuf();
		sh_file.close();
		sh_source_str = sh_stream.str();
	}
	catch (std::ifstream::failure ex)
	{
		std::cout << "ShaderProgram: error reading file!" << std::endl;
	}

	return LoadShader(sh_source_str, stage);
}

GLboolean ShaderProgram::LoadShader(const std::string& sh_source_str, ShaderStage stage)
{
	// Create an empty shader handle
	GLuint shader = glCreateShader((GLuint)stage);

	// Send the shader source code to GL
	const GLchar* sh_source = sh_source_str.c_str();
	glShaderSource(shader, 1, &sh_source, NULL);

	// Compile the shader
	glCompileShader(shader);

	// Check if the shader compiled successfully
	GLint compilation_ok = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compilation_ok);
	if (compilation_ok == GL_FALSE)
	{
		// Get the length of the log string
		GLint maxlen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxlen);

		// Get the log string
		std::vector<GLchar> infolog(maxlen);
		glGetShaderInfoLog(shader, maxlen, &maxlen, &infolog[0]);

		// Delete the shader
		glDeleteShader(shader);

		// Print something useful
		std::cout << "ShaderProgram: error compiling shader";
		switch (stage)
		{
		case ShaderProgram::ShaderStage::VERTEX_SHADER:
			std::cout << " (VERTEX_SHADER): " << std::endl;
			break;
		case ShaderProgram::ShaderStage::GEOMETRY_SHADER:
			std::cout << " (GEOMETRY_SHADER): " << std::endl;
			break;
		case ShaderProgram::ShaderStage::FRAGMENT_SHADER:
			std::cout << " (FRAGMENT_SHADER): " << std::endl;
			break;
		case ShaderProgram::ShaderStage::COMPUTE_SHADER:
			std::cout << " (COMPUTE_SHADER): " << std::endl;
			break;
		default:
			std::cout << " (UNKNOWN): " << std::endl;
			break;
		}
		std::cout << &infolog[0];

		return GL_FALSE;
	}

	// If all is well attach the shader to the program
	glAttachShader(program_, shader);
	attached_shaders_.push_back(shader);

	return GL_TRUE;
}

GLboolean ShaderProgram::Link()
{
	// Check if any shaders are attached, if not, do nothing
	if (attached_shaders_.empty())
	{
		return GL_FALSE;
	}

	// Link the program
	glLinkProgram(program_);

	// Check for errors
	GLint link_ok = GL_FALSE;
	glGetProgramiv(program_, GL_LINK_STATUS, &link_ok);
	if (link_ok == GL_FALSE)
	{
		// Get the lenth of the info log
		GLint maxlen = 0;
		glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &maxlen);

		// Get the info log
		std::vector<GLchar> infolog(maxlen);
		glGetProgramInfoLog(program_, maxlen, &maxlen, &infolog[0]);

		// Delete the program and any attached shaders
		glDeleteProgram(program_);
		for (GLint i = 0; i < attached_shaders_.size(); ++i)
		{
			glDeleteShader(attached_shaders_.at(i));			
		}
		attached_shaders_.resize(0);

		// Recreate the program 
		program_ = glCreateProgram();

		// Print something useful
		std::cout << "ShaderProgram: error linking the attached shaders:"
			<< std::endl << &infolog[0];

		return GL_FALSE;
	}

	// If all is ok then detach and delete the shaders
	for (GLint i = 0; i < attached_shaders_.size(); ++i)
	{
		glDetachShader(program_, attached_shaders_.at(i));
		glDeleteShader(attached_shaders_.at(i));
	}
	attached_shaders_.resize(0);

	// Load some common uniform locations
	model_uniform_			= GetModelUniformLocation()			;
	view_uniform_			= GetViewUniformLocation()			;
	projection_uniform_		= GetProjectionUniformLocation()	;
	view_position_uniform_	= GetViewPositionUniformLocation()	;

	return GL_TRUE;
}

GLvoid ShaderProgram::Bind() const
{
	glUseProgram(program_);
}

GLvoid ShaderProgram::Unbind() const
{
	glUseProgram(0);
}

GLuint ShaderProgram::ID() const
{
	return program_;
}

GLuint ShaderProgram::ModelUniformLocation() const
{
	return model_uniform_;
}

GLuint ShaderProgram::ViewUniformLocation() const
{
	return view_uniform_;
}

GLuint ShaderProgram::ProjectionUniformLocation() const
{
	return projection_uniform_;
}

GLuint ShaderProgram::ViewPositionUniformLocation() const
{
	return view_position_uniform_;
}

GLuint ShaderProgram::GetModelUniformLocation() 
{
	return glGetUniformLocation(program_, "u_model");
}

GLuint ShaderProgram::GetViewUniformLocation()
{
	return glGetUniformLocation(program_, "u_view");
}

GLuint ShaderProgram::GetProjectionUniformLocation()
{
	return glGetUniformLocation(program_, "u_projection");
}

GLuint ShaderProgram::GetViewPositionUniformLocation()
{
	return glGetUniformLocation(program_, "u_viewpos");
}

GLvoid ShaderProgram::SetUniform1i(const GLchar* uniform_name, GLint value)
{
	Bind();
	glUniform1i(
		glGetUniformLocation(program_, uniform_name),
		value
	);
	Unbind();
}