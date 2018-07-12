/**
 * @file Texture.cpp
 * @brief Loading and configuration of textures in general
 */

// Includes C/C++
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Own includes
#include "Texture.h"
#include "Debug.h"
#include "Renderer.h"

// Defines for DDS loading
#define FOURCC_DXT1 0x31545844
#define FOURCC_DXT3 0x33545844
#define FOURCC_DXT5 0x35545844

namespace AMG {

/**
 * @brief Constructor for a Texture (1 frame version)
 * @param path File to load as a Texture
 */
Texture::Texture(const char *path, float bias){
	loadTexture(path, bias);
	this->currentFrame = 0.0f;
	this->texScale.x = 1.0f;
	this->texScale.y = 1.0f;
	this->texPosition.x = 0.0f;
	this->texPosition.y = 0.0f;
	this->horizontalFrames = 1;
	this->verticalFrames = 1;
	this->nframes = 1;
}

/**
 * @brief Constructor for a Texture (cube map version)
 * @param names Path of all the textures, in this order: right, left, up, bottom, behind, front
 * @note Each texture must have the same number of mipmaps (only 1 is recommended)
 */
Texture::Texture(const char **names){

	this->currentFrame = 0.0f;
	this->texScale.x = 1.0f;
	this->texScale.y = 1.0f;
	this->texPosition.x = 0.0f;
	this->texPosition.y = 0.0f;
	this->horizontalFrames = 1;
	this->verticalFrames = 1;
	this->nframes = 1;
	this->target = GL_TEXTURE_CUBE_MAP;

	glGenTextures(1, &this->id);
	glBindTexture(target, this->id);

	int w, h;
	for(int i=0;i<AMG_CUBE_SIDES;i++){
		Texture::loadTexture(names[i], GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, &w, &h);
	}

	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

/**
 * @brief Constructor for a Texture
 * @param path File to load as a Texture
 * @param frameWidth Width of one frame, in pixels
 * @param frameHeight Height of one frame, in pixels
 */
Texture::Texture(const char *path, int frameWidth, int frameHeight, float bias){
	loadTexture(path, bias);
	this->currentFrame = 0.0f;
	float w = (float)frameWidth / (float)width;
	float h = (float)frameHeight / (float)height;
	this->texScale = vec4(w, h, w, h);
	this->texPosition = vec4(0, 0, 0, 0);
	this->horizontalFrames = width / frameWidth;
	this->verticalFrames = height / frameHeight;
	this->nframes = horizontalFrames * verticalFrames;
}

/**
 * @brief Load a texture on memory
 * @param path Location of the texture file (*.dds)
 * @param bias Level of detail bias
 */
void Texture::loadTexture(const char *path, float bias){

	this->target = GL_TEXTURE_2D;
	glGenTextures(1, &this->id);
	glBindTexture(target, this->id);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(target, GL_TEXTURE_LOD_BIAS, bias);

	Texture::loadTexture(path, target, &this->width, &this->height);
}

/**
 * @brief Load a texture on memory
 * @param path Location of the texture file (*.dds)
 * @param w Width of the image (output)
 * @param h Height of the image (output)
 */
void Texture::loadTexture(const char *path, GLuint target, int *w, int *h){
	FILE *fp = fopen(getFullPath(path, AMG_TEXTURE), "rb");
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

	*h    	  			     = *(unsigned int*)&(header[8 ]);
	*w						 = *(unsigned int*)&(header[12]);
	unsigned int linearSize  = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC      = *(unsigned int*)&(header[80]);
	unsigned int width = *w;
	unsigned int height = *h;

	unsigned char * buffer;
	unsigned int bufsize;

	if(height % 2 != 0 || width % 2 != 0){
		fclose(fp);
		Debug::showError(12, (void*)path);
	}

	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*) malloc (bufsize * sizeof(unsigned char));
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

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level){
		unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
		glCompressedTexImage2D(target, level, format, width, height, 0, size, buffer + offset);
		offset += size;
		width  /= 2;
		height /= 2;
	}

	free(buffer);
}

/**
 * @brief Enable texture in OpenGL
 * @param slot Texture slot to upload the texture
 */
void Texture::enable(int slot){

	// Enable the texture
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(this->target, this->id);

	// Perform animation
	if(this->nframes > 1){

		// Calculate current frame as an integer
		int fr = (int)this->currentFrame;
		if(fr >= this->nframes){
			this->currentFrame = 0.0f;
			fr = 0;
		}
		int nfr = fr + 1;
		if(nfr > this->nframes){
			nfr = 0;
		}

		// Update texture position and scale to select the frame
		this->texPosition.x = (fr % horizontalFrames) / (float)horizontalFrames;
		this->texPosition.y = (fr / verticalFrames) / (float)verticalFrames;
		this->texPosition.z = (nfr % horizontalFrames) / (float)horizontalFrames;
		this->texPosition.w = (nfr / verticalFrames) / (float)verticalFrames;
	}

	// Update uniforms in the shader
	Shader *shader = Renderer::currentRenderer->currentShader;
	shader->setUniform("AMG_TexPosition", texPosition);
	shader->setUniform("AMG_TexScale", texScale);
	float p;
	shader->setUniform("AMG_TexProgress", modf(currentFrame, &p));
}

/**
 * @brief Get texture's width, in pixels
 * @return The actual texture width
 */
int Texture::getWidth(){
	return this->width;
}

/**
 * @brief Get texture's height, in pixels
 * @return The actual texture height
 */
int Texture::getHeight(){
	return this->height;
}

/**
 * @brief Get texture's number of frames
 * @return The texture's number of frames
 */
int Texture::getFrames(){
	return this->nframes;
}

/**
 * @brief Destructor for a Texture
 */
Texture::~Texture() {
	glDeleteTextures(1, &this->id);
}

}
