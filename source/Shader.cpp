/**
 * @file Shader.cpp
 * @brief Description of shader loading and manipulation
 */

// Includes C/C++
#include <fstream>
#include <sstream>
#include <vector>

// Includes OpenGL
#define GLEW_STATIC
#include <GL/glew.h>

// Own includes
#include "Shader.h"
#include "Debug.h"
#include "Renderer.h"

namespace AMG {

/**
 * @brief Constructor for a Shader object
 * @param vertex_file_path Location of the vertex shader file, any extension allowed
 * @param fragment_file_path Location of the fragment shader file, any extension allowed
 * @todo Optimise shader code loading
 */
Shader::Shader(const char *vertex_file_path, const char *fragment_file_path) {

	// Create shader objects
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if(VertexShaderID == 0 || FragmentShaderID == 0){
		Debug::showError(4, NULL);
	}

	// Load vertex shader code
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}else{
		Debug::showError(5, (void*)vertex_file_path);
	}

	// Load fragment shader code
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}else{
		Debug::showError(5, (void*)fragment_file_path);
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile vertex shader
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if(InfoLogLength > 0){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		Debug::showError(6, &VertexShaderErrorMessage[0]);
	}

	// Compile fragment shader
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if(InfoLogLength > 0){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		Debug::showError(6, &FragmentShaderErrorMessage[0]);
	}

	// Create a shader program and attach the beforeloaded shaders
	programID = glCreateProgram();
	if(programID == 0)
		Debug::showError(7, NULL);
	glAttachShader(programID, VertexShaderID);
	glAttachShader(programID, FragmentShaderID);

	// Link the shader program
	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &Result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	// Delete shader objects
	glDetachShader(programID, VertexShaderID);
	glDetachShader(programID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	// Show errors
	if(InfoLogLength > 0){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		Debug::showError(8, &ProgramErrorMessage[0]);
	}

	// Everything OK, create a uniform map
	this->uniformsMap = std::tr1::unordered_map<std::string, int>();
	this->defineUniform("MVP");
}

/**
 * @brief Define a uniform variable from the shader
 * @param name Name of the uniform
 */
void Shader::defineUniform(std::string name){
	int id = glGetUniformLocation(programID, name.c_str());
	if(id == -1){
		Debug::showError(9, (void*)name.c_str());
	}
	uniformsMap[name] = id;
}

/**
 * @brief Get a uniform from this shader
 * @param name name of the uniform
 */
int Shader::getUniform(const std::string &name){
	return uniformsMap[name];
}

/**
 * @brief Set a uniform value, 2D vector version
 * @param name Name of the uniform variable to be set
 * @param v Value to set
 */
void Shader::setUniform(const std::string &name, vec2 &v){
	glUniform2f(getUniform(name), v.x, v.y);
}

/**
 * @brief Set a uniform value, 4D vector version
 * @param name Name of the uniform variable to be set
 * @param v Value to set
 */
void Shader::setUniform(const std::string &name, vec4 &v){
	glUniform4f(getUniform(name), v.x, v.y, v.z, v.w);
}

/**
 * @brief Enable a shader program
 */
void Shader::enable(){
	glUseProgram(programID);
	Renderer::shader = this;
}

/**
 * @brief Destructor for a shader object
 */
Shader::~Shader() {
	glDeleteProgram(programID);
	uniformsMap.clear();
}

}
