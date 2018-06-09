/*
 * Shader.h
 *
 *  Created on: 2 jun. 2018
 *      Author: Andrés
 */

#ifndef SHADER_H_
#define SHADER_H_

namespace AMG {

class Shader {
private:
	int programID;
	int matrixID;
public:
	Shader();
	int load(const char *vertex_file_path, const char *fragment_file_path);
	void enable();
	int getMVPLocation();
	virtual ~Shader();
};

} /* namespace AMG */

#endif /* SHADER_H_ */
