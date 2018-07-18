/**
 * @file Shader.h
 * @brief Description of shader loading and manipulation
 */

#ifndef SHADER_H_
#define SHADER_H_

// Includes C/C++
#include <string>
#include <tr1/unordered_map>
#include <vector>

// OpenGL includes
#include <glm/glm.hpp>
using namespace glm;

// Own includes
#include "Entity.h"
#include "Light.h"

namespace AMG {

#define AMG_CUSTOM_SHADER	(0)
#define AMG_USE_SKINNING 	(1 << 0)
#define AMG_USE_FOG 		(1 << 1)
#define AMG_USE_LIGHTING(n)	(((n)&31) << 2)
#define AMG_USE_2D			(1 << 7)
#define AMG_USE_TEXTURE(n)	(((n)&15) << 8)
#define AMG_USE_TEXT		(1 << 13)
#define AMG_USE_TEXANIM		(1 << 14)
#define AMG_USE_INSTANCES	(1 << 15)

/**
 * @class Shader
 * @brief Holds a shader program and its usage
 */
class Shader : private Entity {
private:
	int programID;											/**< Internal OpenGL program ID */
	std::tr1::unordered_map<std::string, int> uniformsMap;	/**< Hash map holding uniform variables in the shader */
	std::vector<Light*> lights;								/**< Vector holding all lights used in the shader */
	int loadShader(const char *path, int type);
	std::string loadShaderCode(const char *path);
public:
	std::vector<Light*> &getLights(){ return lights; }
	int getProgram(){ return programID; }

	Shader(const char *vertex_file_path, const char *fragment_file_path, const char *geometry_file_path, int options);
	void enableOptions(int options);
	void defineUniform(std::string name);
	int getUniform(const std::string &name);
	void setUniform(const std::string &name, int v);
	void setUniform(const std::string &name, float v);
	void setUniform(const std::string &name, vec2 &v);
	void setUniform(const std::string &name, vec3 &v);
	void setUniform(const std::string &name, vec4 &v);
	void setUniform(const std::string &name, mat4 &v);
	void enable();
	virtual ~Shader();
};

}

#endif
