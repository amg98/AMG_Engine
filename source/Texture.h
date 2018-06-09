/*
 * Texture.h
 *
 *  Created on: 3 jun. 2018
 *      Author: Andrés
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

namespace AMG {

class Texture {
private:
	GLuint id;
	int width;
	int height;
public:
	Texture();
	int load(const char *path);
	void enable();
	int getWidth();
	int getHeight();
	virtual ~Texture();
};

} /* namespace AMG */

#endif /* TEXTURE_H_ */
