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

std::string Shader::loadShaderCode(const char *path){
	std::ifstream ShaderStream(path, std::ios::in);
	std::string ShaderCode;
	if(ShaderStream.is_open()){
		std::string line;
		std::stringstream sstr;
		while (std::getline(ShaderStream, line)){
			if(line.find("#include") != std::string::npos){
				int start = line.find("<") + 1;
				int end = line.find(">");
				sstr << loadShaderCode(("Data/Shader/" + line.substr(start, end - start)).c_str());
			}else{
				sstr << line + "\n";
			}
		}
		ShaderCode = sstr.str();
		ShaderStream.close();
	}else{
		Debug::showError(5, (void*)path);
	}
	return ShaderCode;
}

/**
 * @brief Load a shader, used internally
 * @param path Shader code file path
 * @param type GL_VERTEX_SHADER, GL_FRAGMENT_SHADER or GL_GEOMETRY_SHADER
 * @param ShaderCode The actual shader code
 */
int Shader::loadShader(const char *path, int type){

	// Create shader objects
	GLuint id = glCreateShader(type);
	if(id == 0){
		Debug::showError(4, NULL);
	}

	// Load shader code
	std::string ShaderCode = loadShaderCode(path);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile shader
	char const *SourcePointer = ShaderCode.c_str();
	glShaderSource(id, 1, &SourcePointer, NULL);
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
 * @param options The shader packages used, see the defined macros
 */
Shader::Shader(const char *vertex_file_path, const char *fragment_file_path, int options) {

	// Create shader objects
	GLuint VertexShaderID = loadShader(vertex_file_path, GL_VERTEX_SHADER);
	GLuint FragmentShaderID = loadShader(fragment_file_path, GL_FRAGMENT_SHADER);

	// Create a shader program and attach the loaded shaders
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
	lights = std::vector<Light*>();
	this->defineUniform("AMG_MVP");
	this->enableOptions(options);
}

/**
 * @brief Enable shader options to use with the engine
 * @param options The options to use
 */
void Shader::enableOptions(int options){
	if(options &AMG_USE_SKINNING){
		this->defineUniform("AMG_BoneMatrix");
	}
	if(options &AMG_USE_FOG){
		this->defineUniform("AMG_FogDensity");
		this->defineUniform("AMG_FogGradient");
		this->defineUniform("AMG_FogColor");
	}
	int nlights = (options >> 2) &31;
	char text[64];
	if(nlights){
		for(int i=0;i<nlights;i++){
			sprintf(text, "AMG_Light[%d].position", i);
			this->defineUniform(std::string(text));			// Vertex shader
			sprintf(text, "AMG_Lights[%d].color", i);
			this->defineUniform(std::string(text));			// Fragmen shader
		}
		this->defineUniform("AMG_MV");
		this->defineUniform("AMG_MaterialDiffuse");
		this->defineUniform("AMG_MaterialAmbient");
		this->defineUniform("AMG_DiffusePower");
		if(options &AMG_USE_SPECULAR){
			this->defineUniform("AMG_MaterialSpecular");
			this->defineUniform("AMG_SpecularPower");
		}
	}
	if(options &AMG_USE_2D){
		this->defineUniform("AMG_TexPosition");
		this->defineUniform("AMG_TexScale");
		this->defineUniform("AMG_SprColor");
	}
	int ntextures = (options >> 9) &0x15;
	if(ntextures){
		int prg = 0;
		if(Renderer::shader)
			prg = Renderer::shader->getProgram();
		glUseProgram(programID);
		for(int i=0;i<ntextures;i++){
			sprintf(text, "AMG_TextureSampler[%d]", i);
			std::string name = std::string(text);
			this->defineUniform(name);
			this->setUniform(name, i);
		}
		glUseProgram(prg);
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
 * @brief Set a uniform value, int version
 * @param name Name of the uniform variable to be set
 * @param v Value to set
 */
void Shader::setUniform(const std::string &name, int v){
	glUniform1i(getUniform(name), v);
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
	for(unsigned int i=0;i<lights.size();i++){
		lights[i]->enable(i);
	}
}

/**
 * @brief Get this shader's program ID
 * @return The shader ID
 */
int Shader::getProgram(){
	return this->programID;
}

/**
 * @brief Destructor for a shader object
 */
Shader::~Shader() {
	glDeleteProgram(programID);
	uniformsMap.clear();
	lights.clear();
}

}
