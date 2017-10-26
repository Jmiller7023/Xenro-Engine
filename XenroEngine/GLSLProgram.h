#ifndef XENRO_GLSLPROGRAM_DEFINED
#define XENRO_GLSLPROGRAM_DEFINED

#include <string>
#include <GL\glew.h>

namespace Xenro {
	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();

		void compileShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

		void compileShadersFromSource(const char* vertexShader, const char* fragmentShader);

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
		void compileShader(const char* source, const std::string& name, GLuint id);
	};
}

#endif //XENRO_GLSLPROGRAM_DEFINED