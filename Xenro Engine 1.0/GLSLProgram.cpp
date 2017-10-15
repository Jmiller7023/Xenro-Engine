#include "GLSLProgram.h"
#include "Errors.h"
#include <fstream>
#include <string>
#include <vector>

GLSLProgram::GLSLProgram()
	: m_numAttributes(0), m_programID(0), m_fragmentShaderID(0), m_vertexShaderID(0)
{
}


GLSLProgram::~GLSLProgram()
{
}

void GLSLProgram::compileShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
	//Vertex and fragment shaders are successfully compiled.
	//Now time to link them together into a program.
	//Get a program object.
	m_programID = glCreateProgram();

	m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

	if (m_vertexShaderID == 0) {
		fatalError("Failed to create Vertex Shader!");
	}

	m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	if (m_fragmentShaderID == 0) {
		fatalError("Failed to create Fragment Shader!");
	}

	
	compileShader(vertexShaderPath, m_vertexShaderID);
	compileShader(fragmentShaderPath, m_fragmentShaderID);

}

void GLSLProgram::compileShader(const std::string& filePath, GLuint id) {
	std::ifstream vertexFile(filePath);
	if (vertexFile.fail()) {
		fatalError("Failed to open " + filePath);
	}

	std::string line;
	std::string filecontents = "";

	while (std::getline(vertexFile, line)) {
		filecontents += line + "\n";
	}

	vertexFile.close();

	const char* contentsPtr = filecontents.c_str();
	glShaderSource(id, 1, &contentsPtr, nullptr);

	glCompileShader(id);

	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE) {
		GLint maxlength = 1;
		glGetShaderiv(id, maxlength, &maxlength);

		//The maxlength includes the NULL character
		std::vector<char> errorLog(maxlength);
		glGetShaderInfoLog(id, maxlength, &maxlength, &errorLog[0]);

		//Provide the infolog in whatever manor you deem best
		//Exit with failure
		glDeleteShader(id); //Don't create memory leak

		std::printf("%s\n", &(errorLog[0]));
		fatalError("Shader " + filePath + " failed to compile!");
	}

}

void GLSLProgram::linkShaders() {

	//Attach our shaders to our program
	glAttachShader(m_programID, m_vertexShaderID);
	glAttachShader(m_programID, m_fragmentShaderID);

	//Link our program
	glLinkProgram(m_programID);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(m_programID, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 1;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);

		//We don't need the program anymore.
		glDeleteProgram(m_programID);
		//Don't leak shaders either.
		glDeleteShader(m_vertexShaderID);
		glDeleteShader(m_fragmentShaderID);

		std::printf("%s\n", &(errorLog[0]));
		fatalError("Shader failed to link!");
	}

	//Always detach shaders after a successful link.
	glDetachShader(m_programID, m_vertexShaderID);
	glDetachShader(m_programID, m_fragmentShaderID);
	glDeleteShader(m_fragmentShaderID);
	glDeleteShader(m_vertexShaderID);
}

//adds an attribute to the shader. Needs to be called between compiling and linking.
void GLSLProgram::addAttribute(const std::string &attributeName) {
	glBindAttribLocation(m_programID, m_numAttributes++, attributeName.c_str());
}

GLint GLSLProgram::getUniformLocation(const std::string& uniformName) {
	GLint location = glGetUniformLocation(m_programID, uniformName.c_str());
	if (location == GL_INVALID_INDEX) {
		fatalError("Uniform " + uniformName + " not found in Shader!");
	}
	return location;
}

void GLSLProgram::use() {
	glUseProgram(m_programID);
	for (int i = 0; i < m_numAttributes; i++) {
		glEnableVertexAttribArray(i);
	}
}

//enable the shader, and all its attributes.
void GLSLProgram::unuse() {
	glUseProgram(0);

	//enable all the attibutes we added with addAttribute.
	for (int i = 0; i < m_numAttributes; i++) {
		glDisableVertexAttribArray(i);
	}
}