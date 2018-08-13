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
 * @brief Default constructor for a Texture
 */
Texture::Texture(){
	this->target = GL_TEXTURE_2D;
	this->progress = 0;
	this->width = 0;
	this->height = 0;
	this->currentFrame = 0;
	this->texScale = vec2(1, 1);
	this->texPosition = vec4(0, 0, 0, 0);
	this->horizontalFrames = 1;
	this->verticalFrames = 1;
	this->nframes = 1;
	this->id = 0;
	this->isCopy = false;
}

/**
 * @brief Create an empty cube map texture
 * @param dimensions Width and height for each cube face, in pixels
 */
void Texture::createCubeMap(int dimensions){
	this->target = GL_TEXTURE_CUBE_MAP;
	this->width = dimensions;
	this->height = dimensions;

	glGenTextures(1, &this->id);
	glBindTexture(target, this->id);

	for(int i=0;i<AMG_CUBE_SIDES;i++){
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, dimensions, dimensions, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}

	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(target, 0);
}

/**
 * @brief Copies information from another texture
 * @param texture Texture to copy
 */
void Texture::set(Texture *texture){
	this->target = texture->target;
	this->progress = texture->progress;
	this->width = texture->getWidth();
	this->height = texture->getHeight();
	this->currentFrame = texture->getCurrentFrame();
	this->texScale = texture->texScale;
	this->texPosition = texture->texPosition;
	this->horizontalFrames = texture->horizontalFrames;
	this->verticalFrames = texture->verticalFrames;
	this->nframes = texture->getNFrames();
	this->id = texture->id;
	this->isCopy = true;
}

/**
 * @brief Constructor for a Texture using another Texture (so it makes a copy)
 * @param texture Texture to copy
 */
Texture::Texture(Texture *texture){
	set(texture);
}

/**
 * @brief Constructor for a Texture (assigns it to the current Framebuffer)
 * @param w Width of the texture, in pixels
 * @param h Height of the texture, in pixels
 * @param mode How to generate the texture: GL_RGB/GL_DEPTH_COMPONENT32
 * @param mode2 Second mode for glTexImage2D
 * @param attachment Where to attach the texture
 * @param type Texture type (GL_UNSIGNED_BYTE as default)
 */
Texture::Texture(int w, int h, GLuint mode, GLuint mode2, GLuint attachment, GLuint type){
	this->target = GL_TEXTURE_2D;
	this->progress = 0.0f;
	this->width = w;
	this->height = h;
	this->currentFrame = 0.0f;
	this->texScale = vec2(1, 1);
	this->texPosition = vec4(0, 0, 0, 0);
	this->horizontalFrames = 1;
	this->verticalFrames = 1;
	this->nframes = 1;
	this->width = w;
	this->height = h;
	this->isCopy = false;
	glGenTextures(1, &id);
	glBindTexture(target, id);
	glTexImage2D(target, 0, mode, w, h, 0, mode2, type, NULL);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, target, id, 0);
	glBindTexture(target, 0);
}

/**
 * @brief Constructor for a Texture (1 frame version)
 * @param path File to load as a Texture
 */
Texture::Texture(const char *path){
	loadTexture(path);
	this->currentFrame = 0.0f;
	this->texScale = vec2(1, 1);
	this->texPosition = vec4(0, 0, 0, 0);
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
	this->texPosition = vec4(0, 0, 0, 0);
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

	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(target, 0);
}

/**
 * @brief Constructor for a Texture
 * @param path File to load as a Texture
 * @param frameWidth Width of one frame, in pixels
 * @param frameHeight Height of one frame, in pixels
 */
Texture::Texture(const char *path, int frameWidth, int frameHeight){
	loadTexture(path);
	this->currentFrame = 0.0f;
	this->texScale.x = (float)frameWidth / (float)width;
	this->texScale.y = (float)frameHeight / (float)height;
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
void Texture::loadTexture(const char *path){

	this->target = GL_TEXTURE_2D;
	glGenTextures(1, &this->id);
	glBindTexture(target, this->id);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	Texture::loadTexture(path, target, &this->width, &this->height);
	glBindTexture(target, 0);
}

/**
 * @brief Set texture level of detail
 * @param bias LOD bias
 */
void Texture::setLod(float bias){
	glBindTexture(target, this->id);
	if(GLEW_EXT_texture_lod_bias){
		glTexParameterf(target, GL_TEXTURE_LOD_BIAS, bias);
	}
}

/**
 * @brief Set texture anisotropic filtering
 * @param aniso Amount of anisotropic filtering
 */
void Texture::setAniso(float aniso){
	if(GLEW_ARB_texture_filter_anisotropic){
		float maxaniso = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxaniso);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, glm::min(aniso, maxaniso));
	}
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
 * @brief Bind a texture
 * @param slot Texture slot to upload the texture
 */
void Texture::bind(int slot){

	// Bind the texture
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(this->target, this->id);

	// Update uniforms in the shader
	Shader *shader = Renderer::getCurrentShader();
	shader->setUniform("AMG_TexPosition", texPosition);
	shader->setUniform("AMG_TexScale", texScale);
	shader->setUniform("AMG_TexProgress", progress);
}

/**
 * @brief Unbind a texture
 * @param slot Texture slot to upload the texture
 */
void Texture::unbind(int slot){
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(this->target, 0);
}

/**
 * @brief Animate a texture
 */
void Texture::animate(){

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
		float p;
		progress = modf(currentFrame, &p);
	}
}

/**
 * @brief Flush frame data to a float buffer
 * @param data Float buffer
 * @param offset Where to start storing the data (in floats)
 */
void Texture::storeFrameData(float *data, int offset){
	data[offset + 0] = texPosition.x;
	data[offset + 1] = texPosition.y;
	data[offset + 2] = texPosition.z;
	data[offset + 3] = texPosition.w;
	data[offset + 4] = progress;
}

/**
 * @brief Destructor for a Texture
 */
Texture::~Texture() {
	if(this->id && !isCopy) glDeleteTextures(1, &this->id);
}

}
