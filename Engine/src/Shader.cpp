#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "glm/glm.hpp"

Shader::Shader(const char* vertexPath, const char* fragmentPath, bool isFile)
{
	std::string vertexCode;
	std::string fragmentCode;
	if(isFile)
	{
		vertexCode = LoadShaderSource(vertexPath);
		fragmentCode = LoadShaderSource(fragmentPath);
	}
	else
	{
		vertexCode = vertexPath;
		fragmentCode = fragmentPath;
	}

	const GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexCode.c_str());
	const GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());

	m_ShaderProgram = glCreateProgram();
	glAttachShader(m_ShaderProgram,vertexShader);
	glAttachShader(m_ShaderProgram,fragmentShader);
	glLinkProgram(m_ShaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(m_ShaderProgram);
}

void Shader::Use()
{
	glUseProgram(m_ShaderProgram);
}

void Shader::SetUniform(const char* name, glm::mat4 mat) const
{
	const GLint location = glGetUniformLocation(m_ShaderProgram, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetUniform(const char* name, glm::vec3 vec) const
{
	const GLint location = glGetUniformLocation(m_ShaderProgram, name);
	glUniform3fv(location, 1, &vec[0]);
}

void Shader::SetUniform(const char* name, float value) const
{
	const GLint location = glGetUniformLocation(m_ShaderProgram, name);
	glUniform1f(location, value);
}

void Shader::SetUniform(const char* name, int value) const
{
	const GLint location = glGetUniformLocation(m_ShaderProgram, name);
	glUniform1i(location, value);
}

GLuint Shader::CompileShader(GLenum type, const char* source)
{
	const GLuint shader{ glCreateShader(type) };
	glShaderSource(shader, 1, &source,nullptr);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		throw std::runtime_error(infoLog);
	}
	return shader;
}

std::string Shader::LoadShaderSource(const char* path)
{
	std::ifstream file{path};
	if(!file.is_open())
	{
		throw std::runtime_error("Failed to open file!");
	}
	std::stringstream ss;
	ss << file.rdbuf();

	if(ss.str().empty())
	{
		throw std::runtime_error("Failed to read from file!");
	}

	std::string content = ss.str();
	return content;
}
