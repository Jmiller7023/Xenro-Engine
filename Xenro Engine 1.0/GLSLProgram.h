#pragma once
#include <string>
#include <GL\glew.h>
class GLSLProgram
{
public:
	GLSLProgram();
	~GLSLProgram();

	void compileShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

	void linkShaders();

	void addAttribute(const std::string &attributeName);

	GLint getUniformLocation(const std::string& uniformName);

	void use();

	void unuse();
private:
	int m_numAttributes;
	GLuint m_programID;
	GLuint m_vertexShaderID;
	GLuint m_fragmentShaderID;
	void compileShader(const std::string& filePath, GLuint id);
};

