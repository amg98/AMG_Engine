/**
 * @file Shader.cpp
 * @brief Description of shader loading and manipulation
 */

// Includes C/C++
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// Own includes
#include "Shader.h"
#include "Debug.h"
#include "Renderer.h"
#include "WaterTile.h"

namespace AMG {

// Internal data
const char *Shader::uniformsTable[] = {
	"AMG_MVP", "AMG_BoneMatrix",
	"AMG_NLights", "AMG_MV", "AMG_M",
	"AMG_FogDensity", "AMG_FogGradient", "AMG_CamPosition", "AMG_TexScale", "AMG_TexPosition",
	"AMG_ShadowMatrix", "AMG_ShadowDistance", "AMG_ShadowMapSize", "AMG_ClippingPlanes",
	"AMG_MaterialDiffuse", "AMG_MaterialAmbient", "AMG_MaterialSpecular", "AMG_DiffusePower",
	"AMG_SpecularPower", "AMG_SprColor", "AMG_FogColor", "AMG_TexProgress", "AMG_CharWidth",
	"AMG_CharEdge", "AMG_CharBorderWidth", "AMG_CharBorderEdge", "AMG_CharShadowOffset",
	"AMG_CharOutlineColor", "AMG_SSAOSamples", "AMG_SSAOProjection", "AMG_DView", "AMG_HDRExposure",
	"AMG_GammaValue", "AMG_SpecularReflectivity", "AMG_SSAOKernelSize", "AMG_SSAOKernelRadius", "AMG_WorldAmbient",
	"AMG_RefractionIndex",
};

/**
 * @brief Load a file onto a string, doing preprocessing step
 * @param path Path to the file to be loaded
 * @return The code onto that file on a std::string
 */
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
				sstr << loadShaderCode(getFullPath(line.substr(start, end - start).c_str(), AMG_SHADERLIB));
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
 */
int Shader::loadShader(const char *path, int type){

	// Create shader objects
	GLuint id = glCreateShader(type);
	if(id == 0){
		Debug::showError(4, NULL);
	}

	// Load shader code
	std::string ShaderCode = loadShaderCode(getFullPath(path, AMG_SHADER));

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
		fprintf(stderr, "[%s]\n", path);
		Debug::showError(6, &ShaderErrorMessage[0]);
	}

	return id;
}

/**
 * @brief Constructor for a Shader object
 * @param file_path Shader file path (without extension)
 * @note Vertex shaders must have a .vs extension, fragment shaders .fs, and geometry shaders .gs
 */
Shader::Shader(const char *file_path) {

	// Create shader objects
	char path[512];
	sprintf(path, "%s.vs", file_path);
	GLuint VertexShaderID = loadShader(path, GL_VERTEX_SHADER);
	sprintf(path, "%s.fs", file_path);
	GLuint FragmentShaderID = loadShader(path, GL_FRAGMENT_SHADER);

	// Check the geometry shader
	GLuint GeometryShaderID = 0;
	sprintf(path, "%s.gs", file_path);
	FILE *f = fopen(path, "rb");
	if(f){
		fclose(f);
		GeometryShaderID = loadShader(path, GL_GEOMETRY_SHADER);
	}

	// Create a shader program and attach the loaded shaders
	programID = glCreateProgram();
	if(programID == 0)
		Debug::showError(7, NULL);
	glAttachShader(programID, VertexShaderID);
	glAttachShader(programID, FragmentShaderID);
	if(GeometryShaderID) glAttachShader(programID, GeometryShaderID);

	// Link the shader program
	glLinkProgram(programID);

	GLint Result = GL_FALSE;
	int InfoLogLength;
	glGetProgramiv(programID, GL_LINK_STATUS, &Result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	// Delete shader objects
	glDetachShader(programID, VertexShaderID);
	glDetachShader(programID, FragmentShaderID);
	if(GeometryShaderID) glDetachShader(programID, GeometryShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
	if(GeometryShaderID) glDeleteShader(GeometryShaderID);

	// Show errors
	if(InfoLogLength > 0){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(stderr, "[%s]\n", file_path);
		Debug::showError(8, &ProgramErrorMessage[0]);
	}

	// Everything OK, create a uniform map
	this->uniformsMap = std::tr1::unordered_map<std::string, int>();

	// Define all uniforms
	unsigned int nuniforms = sizeof(uniformsTable)/sizeof(char*);
	for(unsigned int i=0;i<nuniforms;i++){
		internalDefineUniform(uniformsTable[i]);
	}
	char text[64];
	for(int i=0;i<AMG_MAX_LIGHTS;i++){
		sprintf(text, "AMG_Light[%d].position", i);
		internalDefineUniform(std::string(text));			// Vertex shader
		sprintf(text, "AMG_Lights[%d].color", i);
		internalDefineUniform(std::string(text));			// Fragment shader
		sprintf(text, "AMG_Lights[%d].attenuation", i);
		internalDefineUniform(std::string(text));
		sprintf(text, "AMG_Lights[%d].spotDirection", i);
		internalDefineUniform(std::string(text));
		sprintf(text, "AMG_Lights[%d].spotCutoff", i);
		internalDefineUniform(std::string(text));
	}
	for(int i=0;i<AMG_MAX_DLIGHTS;i++){
		sprintf(text, "AMG_LightDR[%d].position", i);
		internalDefineUniform(std::string(text));
		sprintf(text, "AMG_LightDR[%d].color", i);
		internalDefineUniform(std::string(text));
		sprintf(text, "AMG_LightDR[%d].attenuation", i);
		internalDefineUniform(std::string(text));
		sprintf(text, "AMG_LightDR[%d].spotDirection", i);
		internalDefineUniform(std::string(text));
		sprintf(text, "AMG_LightDR[%d].spotCutoff", i);
		internalDefineUniform(std::string(text));
	}
	glUseProgram(programID);
	for(int i=0;i<AMG_MAX_TEXTURES;i++){
		sprintf(text, "AMG_TextureSampler[%d]", i);
		std::string name = std::string(text);
		internalDefineUniform(name);
		setUniform(name, i);
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
 * @brief Define a uniform variable from the shader
 * @param name Name of the uniform
 * @note This function doesn't pop an error message if not defined
 */
void Shader::internalDefineUniform(std::string name){
	int id = glGetUniformLocation(programID, name.c_str());
	if(id != -1){
		uniformsMap[name] = id;
	}
}

/**
 * @brief Get a uniform from this shader
 * @param name name of the uniform
 * @return the uniform ID, or -1 if it does not exist
 */
int Shader::getUniform(const std::string &name){
	std::tr1::unordered_map<std::string, int>::const_iterator got = uniformsMap.find(name);
	if(got == uniformsMap.end()){
		return -1;
	}
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
 * @brief Set a uniform value, 3x3 matrix version
 * @param name Name of the uniform variable to be set
 * @param v Value to set
 */
void Shader::setUniform(const std::string &name, mat3 &v){
	glUniformMatrix3fv(getUniform(name), 1, GL_FALSE, &v[0][0]);
}

/**
 * @brief Set a uniform value, vec3 array version
 * @param name Name of the uniform variable to be set
 * @param n Number of vectors
 * @param data Value to set
 */
void Shader::setUniform3fv(const std::string &name, int n, GLfloat *data){
	glUniform3fv(getUniform(name), n, data);
}

/**
 * @brief Enable a shader program
 */
void Shader::enable(){
	if(Renderer::getCurrentShader() != this){

		// Enable the shader program
		glUseProgram(programID);
		Renderer::setCurrentShader(this);

		// Update fog uniforms
		setUniform(AMG_FogColor, Renderer::getFogColor());
		setUniform(AMG_FogDensity, Renderer::getFogDensity());
		setUniform(AMG_FogGradient, Renderer::getFogGradient());

		// Update forward lighting uniforms
		std::vector<Light*> &lights = Renderer::getLights();
		for(unsigned int i=0;i<lights.size();i++){
			lights[i]->enable(i);
		}
	}
}

/**
 * @brief Setup a clipping plane for this shader
 * @param id Clipping plane ID (0-7)
 * @param plane Plane equation for the clipping plane
 */
void Shader::setClipPlane(int id, vec4 &plane){
	glEnable(GL_CLIP_DISTANCE0 + id);
	glUniform4f(getUniform("AMG_ClippingPlanes") + id, plane.x, plane.y, plane.z, plane.w);
}

/**
 * @brief Setup a clipping plane for this shader (water rendering)
 * @param plane Plane equation for the clipping plane
 */
void Shader::setWaterClipPlane(vec4 &plane){
	glEnable(GL_CLIP_DISTANCE0 + AMG_WATER_CLIPPING_PLANE);
	glUniform4f(getUniform("AMG_ClippingPlanes") + AMG_WATER_CLIPPING_PLANE, plane.x, plane.y, plane.z, plane.w);
}

/**
 * @brief Disables a clipping plane
 * @param id Clipping plane ID (0-7)
 */
void Shader::disableClipPlane(int id){
	glUniform4f(getUniform("AMG_ClippingPlanes") + id, 0, 1, 0, 100000);	// Some gpu's ignore the glDisable call
	glDisable(GL_CLIP_DISTANCE0 + id);
}

/**
 * @brief Disables the water engine's clipping plane
 */
void Shader::disableWaterClipPlane(){
	glUniform4f(getUniform("AMG_ClippingPlanes") + AMG_WATER_CLIPPING_PLANE, 0, 1, 0, 100000);
	glDisable(GL_CLIP_DISTANCE0 + AMG_WATER_CLIPPING_PLANE);
}

/**
 * @brief Destructor for a shader object
 */
Shader::~Shader() {
	glDeleteProgram(programID);
}

}
