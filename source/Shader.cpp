/**
 * @file Shader.cpp
 * @brief Description of shader loading and manipulation
 */

// Includes C/C++
#include <iostream>
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
 * @brief Load a shader, used internally
 * @param path Shader code file path
 * @param type GL_VERTEX_SHADER, GL_FRAGMENT_SHADER or GL_GEOMETRY_SHADER
 * @param ShaderCode The actual shader code
 */
int Shader::loadShader(const char *path, int type, std::string &ShaderCode){

	// Create shader objects
	GLuint id = glCreateShader(type);
	if(id == 0){
		Debug::showError(4, NULL);
	}

	// Load shader code
	std::ifstream ShaderStream(path, std::ios::in);
	if(ShaderStream.is_open()){
		std::stringstream sstr;
		sstr << ShaderStream.rdbuf();
		ShaderCode = sstr.str();
		ShaderStream.close();
	}else{
		Debug::showError(5, (void*)path);
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile shader
	char const *SourcePointer = ShaderCode.c_str();
	glShaderSource(id, 1, &SourcePointer , NULL);
	glCompileShader(id);

	// Get compilation status
	glGetShaderiv(id, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if(InfoLogLength > 0){
		std::vector<char> ShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(id, InfoLogLength, NULL, &ShaderErrorMessage[0]);
		Debug::showError(6, &ShaderErrorMessage[0]);
	}

	return id;
}

/**
 * @brief Constructor for a Shader object
 * @param vertex_file_path Location of the vertex shader file, any extension allowed
 * @param fragment_file_path Location of the fragment shader file, any extension allowed
 */
Shader::Shader(const char *vertex_file_path, const char *fragment_file_path) {

	// Create shader objects
	std::string vertexCode;
	std::string fragmentCode;
	GLuint VertexShaderID = loadShader(vertex_file_path, GL_VERTEX_SHADER, vertexCode);
	GLuint FragmentShaderID = loadShader(fragment_file_path, GL_FRAGMENT_SHADER, fragmentCode);

	// Create a shader program and attach the beforeloaded shaders
	programID = glCreateProgram();
	if(programID == 0)
		Debug::showError(7, NULL);
	glAttachShader(programID, VertexShaderID);
	glAttachShader(programID, FragmentShaderID);

	// Link the shader program
	glLinkProgram(programID);

	GLint Result = GL_FALSE;
	int InfoLogLength;
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
	this->defineUniforms(vertexCode);
	this->defineUniforms(fragmentCode);
}

/**
 * @brief Define all uniforms in a shader code
 * @todo Comments and structs
 */
void Shader::defineUniforms(std::string code){
	int uniformStartLocation = code.find("uniform");
	while(uniformStartLocation != -1){
		int begin = uniformStartLocation + 8;
		int end = code.find(";", begin);
		int b = code.find("[", begin);
		if(b < end && b != -1) end = b;

		std::string line = code.substr(begin, end - begin);
		std::string name = line.substr(line.find(" ") + 1);

		defineUniform(name);

		uniformStartLocation = code.find("uniform", uniformStartLocation + 7);
	}
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
 * @return the uniform ID, or -1 if it does not exist
 */
int Shader::getUniform(const std::string &name){
	std::tr1::unordered_map<std::string, int>::const_iterator got = uniformsMap.find(name);
	if(got == uniformsMap.end())
		return -1;
	return got->second;
}

/**
 * @brief Set a uniform value, float version
 * @param name Name of the uniform variable to be set
 * @param v Value to set
 */
void Shader::setUniform(const std::string &name, float v){
	glUniform1f(getUniform(name), v);
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
 * @brief Set a uniform value, 3D vector version
 * @param name Name of the uniform variable to be set
 * @param v Value to set
 */
void Shader::setUniform(const std::string &name, vec3 &v){
	glUniform3f(getUniform(name), v.x, v.y, v.z);
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
 * @brief Set a uniform value, 4x4 matrix version
 * @param name Name of the uniform variable to be set
 * @param v Value to set
 */
void Shader::setUniform(const std::string &name, mat4 &v){
	glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, &v[0][0]);
}

/**
 * @brief Enable a shader program
 */
void Shader::enable(){
	glUseProgram(programID);
	Renderer::shader = this;
	Renderer::currentRenderer->updateFog();
}

/**
 * @brief Destructor for a shader object
 */
Shader::~Shader() {
	glDeleteProgram(programID);
	uniformsMap.clear();
}

}
