/*
 * Debug.h
 *
 *  Created on: 1 jun. 2018
 *      Author: Andrés
 */

#ifndef DEBUG_H_
#define DEBUG_H_

namespace AMG {

enum ErrorCodes {
	NO_ERROR = 0,
	GLFW_INIT_ERROR,
	GLFW_WINDOW_ERROR,
	GLEW_INIT_ERROR,
	SHADER_CREATION_ERROR,
	FILE_NOT_FOUND,
	SHADER_COMPILATION,
	PROGRAM_CREATION_ERROR,
	SHADER_LINKING,
	UNIFORM_NOT_FOUND,
	WRONG_SIGNATURE,
	UNSUPPORTED_FORMAT,
	NOT_POWER_OF_TWO_TEXTURE,
};

class Debug {
public:
	static void showError(int code, void *param);
};

}

#endif /* DEBUG_H_ */
