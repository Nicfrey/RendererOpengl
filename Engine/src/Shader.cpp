#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

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

	m_VertexShader = CompileShader(GL_VERTEX_SHADER, vertexCode.c_str());
	m_FragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());

	m_ShaderProgram = glCreateProgram();
	glAttachShader(m_ShaderProgram, m_VertexShader);
	CheckError("Attach vertex shader");
	glAttachShader(m_ShaderProgram,m_FragmentShader);
	CheckError("Attach fragment shader");
	glLinkProgram(m_ShaderProgram);
	CheckError("Link shader program");

	glDeleteShader(m_VertexShader);
	glDeleteShader(m_FragmentShader);
}

Shader::~Shader()
{
	glUseProgram(0);
	CheckError("Remove program");
	glDetachShader(m_ShaderProgram,m_VertexShader);
	CheckError("detach vertex shader");
	glDetachShader(m_ShaderProgram,m_FragmentShader);
	CheckError("detach fragment program");
}

void Shader::Use()
{
	glUseProgram(m_ShaderProgram);
}

void Shader::SetUniform(const char* name, glm::mat4 mat) const
{
	const GLint location = glGetUniformLocation(m_ShaderProgram, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
	CheckError("Set uniformMatrix4fv");
}

void Shader::SetUniform(const char* name, glm::vec3 vec) const
{
	const GLint location = glGetUniformLocation(m_ShaderProgram, name);
	glUniform3fv(location, 1, &vec[0]);
	CheckError("Set uniformMatrix3fv");
}

void Shader::SetUniform(const char* name, float value) const
{
	const GLint location = glGetUniformLocation(m_ShaderProgram, name);
	glUniform1f(location, value);
	CheckError("Set uniformMatrix1f");
}

void Shader::SetUniform(const char* name, int value) const
{
	const GLint location = glGetUniformLocation(m_ShaderProgram, name);
	glUniform1i(location, value);
	CheckError("Set uniformMatrix1i");
}

void Shader::CheckError(const char *context)
{
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
	{
		std::cerr << "OpenGL Error (" << error << ") at " << context << "\n";
	}
}

void Shader::DebugBuffer(GLuint buffer, size_t size)
{
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	std::vector<float> data(size / sizeof(float));
	glGetBufferSubData(GL_ARRAY_BUFFER, 0, size, data.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	std::cout << "Buffer Data: ";
	for (size_t i = 0; i < data.size(); ++i)
	{
		std::cout << data[i] << " ";
	}
	std::cout << std::endl;
}

void Shader::DebugEBO(GLuint ebo, size_t size)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	std::vector<GLuint> data(size / sizeof(GLuint));
	glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data.data());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	std::cout << "EBO Data: ";
	for (unsigned int i : data)
	{
		std::cout << i << " ";
	}
	std::cout << "\n";
}

GLuint Shader::CompileShader(GLenum type, const char *source)
{
	const GLuint shader{ glCreateShader(type) };
	CheckError("Creation Shader");
	glShaderSource(shader, 1, &source,nullptr);
	CheckError("Set Shader source");
	glCompileShader(shader);
	CheckError("Compile Shader");

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
