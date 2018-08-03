/*************************************************************************/
/*                           GLSLProgram.h                               */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           Xenro Engine                                */
/*             https://github.com/Jmiller7023/Xenro-Engine               */
/*************************************************************************/
/*                 Copyright  11-3-2017 Joseph Miller.                   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef XENRO_GLSLPROGRAM_DEFINED
#define XENRO_GLSLPROGRAM_DEFINED

#include <string>
#include <GL\glew.h>

namespace Xenro {

class GLSLProgram {
public:
	GLSLProgram();
	~GLSLProgram();

	void compileShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

	void compileShadersFromSource(const char* vertexShader, const char* fragmentShader);

	void linkShaders();

	void addAttribute(const std::string &attributeName);

	GLint getUniformLocation(const std::string& uniformName);

	void use();
	void use(const std::string& uniformName);

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