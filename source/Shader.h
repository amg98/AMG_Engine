/*
 * Shader.h
 *
 *  Created on: 2 jun. 2018
 *      Author: Andrés
 */

#ifndef SHADER_H_
#define SHADER_H_

#include "Entity.h"
#include <tr1/unordered_map>

#include <glm/glm.hpp>
using namespace glm;

namespace AMG {

class Shader : private Entity {
private:
	int programID;
	std::tr1::unordered_map<const char*, int> uniformsMap;
public:
	Shader(const char *vertex_file_path, const char *fragment_file_path);
	void defineUniform(const char *name);
	int getUniform(const char *name);
	void setUniform(const char* name, vec2 &v);
	void setUniform(const char* name, vec4 &v);
	void enable();
	virtual ~Shader();
};

} /* namespace AMG */

#endif /* SHADER_H_ */
