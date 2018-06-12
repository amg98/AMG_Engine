/*
 * Shader.cpp
 *
 *  Created on: 2 jun. 2018
 *      Author: Andrés
 */

#include "Shader.h"
#include "Debug.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace AMG {

Shader::Shader(const char *vertex_file_path, const char *fragment_file_path) {
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if(VertexShaderID == 0 || FragmentShaderID == 0){
		Debug::showError(4, NULL);
	}

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

	programID = glCreateProgram();
	if(programID == 0)
		Debug::showError(7, NULL);
	glAttachShader(programID, VertexShaderID);
	glAttachShader(programID, FragmentShaderID);
	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &Result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	glDetachShader(programID, VertexShaderID);
	glDetachShader(programID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	if(InfoLogLength > 0){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		Debug::showError(8, &ProgramErrorMessage[0]);
	}

	this->uniformsMap = std::tr1::unordered_map<const char*, int>();
	this->defineUniform("MVP");
}

void Shader::defineUniform(const char *name){
	int id = glGetUniformLocation(programID, name);
	if(id == -1){
		Debug::showError(9, (void*)name);
	}
	uniformsMap[name] = id;
}

// Supongo que lanza una excepcion si no lo encuentra
int Shader::getUniform(const char *name){
	return uniformsMap[name];
}

void Shader::setUniform(const char *name, vec2 &v){
	glUniform2f(getUniform(name), v.x, v.y);
}

void Shader::setUniform(const char *name, vec4 &v){
	glUniform4f(getUniform(name), v.x, v.y, v.z, v.w);
}

void Shader::enable(){
	glUseProgram(programID);
}

Shader::~Shader() {
	glDeleteProgram(programID);
}

} /* namespace AMG */
