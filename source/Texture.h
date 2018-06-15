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

#include "Entity.h"

namespace AMG {

class Texture : public Entity {
private:
	GLuint id;
	bool loaded;
protected:
	int width;
	int height;
public:
	Texture(const char *path);
	void enable();
	int getWidth();
	int getHeight();
	virtual ~Texture();
};

} /* namespace AMG */

#endif /* TEXTURE_H_ */
