/**
 * @file Shader.h
 * @brief Description of shader loading and manipulation
 */

#ifndef SHADER_H_
#define SHADER_H_

// Includes C/C++
#include <string>
#include <tr1/unordered_map>

// OpenGL includes
#include <glm/glm.hpp>
using namespace glm;

// Own includes
#include "Entity.h"

namespace AMG {

class Shader : private Entity {
private:
	int programID;											/**< Internal OpenGL program ID */
	std::tr1::unordered_map<std::string, int> uniformsMap;		/**< Hash map holding uniform variables in the shader */
public:
	Shader(const char *vertex_file_path, const char *fragment_file_path);
	void defineUniform(std::string name);
	int getUniform(const std::string &name);
	void setUniform(const std::string &name, vec2 &v);
	void setUniform(const std::string &name, vec4 &v);
	void enable();
	virtual ~Shader();
};

}

#endif
