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
#include <GL/glew.h>
#include <glm/glm.hpp>
using namespace glm;

// Own includes
#include "Entity.h"
#include "Light.h"

// Defines
#define AMG_MAX_LIGHTS 4		/**< Maximum number of lights at once */
#define AMG_MAX_DLIGHTS 8		/**< Maximum number of lights in deferred rendering */
#define AMG_MAX_TEXTURES 6		/**< Maximum number of textures at once */

namespace AMG {

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
	void internalDefineUniform(std::string name);
public:
	std::vector<Light*> &getLights(){ return lights; }
	int getProgram(){ return programID; }

	Shader(const char *vertex_file_path, const char *fragment_file_path, const char *geometry_file_path=NULL);
	void defineUniform(std::string name);
	int getUniform(const std::string &name);
	void setUniform(const std::string &name, int v);
	void setUniform(const std::string &name, float v);
	void setUniform(const std::string &name, vec2 &v);
	void setUniform(const std::string &name, vec3 &v);
	void setUniform(const std::string &name, vec4 &v);
	void setUniform(const std::string &name, mat4 &v);
	void setUniform(const std::string &name, mat3 &v);
	void setUniform3fv(const std::string &name, int n, GLfloat *data);
	void setClipPlane(int id, vec4 &plane);
	void setWaterClipPlane(vec4 &plane);
	void disableClipPlane(int id);
	void disableWaterClipPlane();
	void enable();
	inline void disable(){ glUseProgram(0); }
	virtual ~Shader();
};

}

#endif
