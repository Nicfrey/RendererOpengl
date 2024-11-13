#pragma once
#include "gl/glew.h"
#include <string>

#include "glm/fwd.hpp"

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath, bool isFile = true);
	~Shader();
	void Use();
	void SetUniform(const char* name, glm::mat4 mat) const;
	void SetUniform(const char* name, glm::vec3 vec) const;
	void SetUniform(const char* name, float value) const;
	void SetUniform(const char* name, int value) const;
	Shader(const Shader&) = delete;
	Shader(Shader&&) = delete;
	Shader& operator=(const Shader&) = delete;
	Shader& operator=(Shader&&) = delete;

private:
	GLuint CompileShader(GLenum type, const char* source);
	std::string LoadShaderSource(const char* path);
	GLuint m_ShaderProgram;
};
