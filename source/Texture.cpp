/*
 * Texture.cpp
 *
 *  Created on: 3 jun. 2018
 *      Author: Andrés
 */

#include "Texture.h"
#include "Debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FOURCC_DXT1 0x31545844
#define FOURCC_DXT3 0x33545844
#define FOURCC_DXT5 0x35545844

namespace AMG {

Texture::Texture(const char *path){
	FILE *fp = fopen(path, "rb");
	if (fp == NULL)
		Debug::showError(5, (void*)path);

	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
	    fclose(fp);
	    Debug::showError(10, (void*)path);
	}

	unsigned char header[124];
	fread(&header, 124, 1, fp);

	this->height    	     = *(unsigned int*)&(header[8 ]);
	this->width       	     = *(unsigned int*)&(header[12]);
	unsigned int linearSize  = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC      = *(unsigned int*)&(header[80]);
	unsigned int width = this->width;
	unsigned int height = this->height;

	unsigned char * buffer;
	unsigned int bufsize;

	if(this->height % 2 != 0 || this->width % 2 != 0){
		fclose(fp);
		Debug::showError(12, (void*)path);
	}

	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	fclose(fp);

	//unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch(fourCC){
		case FOURCC_DXT1:
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			break;
		case FOURCC_DXT3:
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
		case FOURCC_DXT5:
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		default:
			free(buffer);
			Debug::showError(11, (void*)path);
	}

	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level){
		unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer + offset);
		offset += size;
		width  /= 2;
		height /= 2;
	}

	free(buffer);
}

void Texture::enable(){
	glBindTexture(GL_TEXTURE_2D, this->id);
}

int Texture::getWidth(){
	return this->width;
}

int Texture::getHeight(){
	return this->height;
}

Texture::~Texture() {
	glDeleteTextures(1, &this->id);
}

} /* namespace AMG */
