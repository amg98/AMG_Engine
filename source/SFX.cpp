/**
 * @file SFX.cpp
 * @brief Defines a sound effect in the audio engine
 */

// Includes C/C++
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Own includes
#include "SFX.h"
#include "Debug.h"

namespace AMG {

/**
 * @brief Constructor for a sound effect
 * @param path Path to the sound effect file
 * @note Only PCM sound data is supported
 */
SFX::SFX(const char *path) {

	// Open the WAV file
	FILE *f = fopen(Entity::getFullPath(path, AMG_AUDIO), "rb");
	if(f == NULL) Debug::showError(FILE_NOT_FOUND, (void*)path);

	// Check the RIFF signature
	char sign[4];
	fread(sign, 4, sizeof(char), f);
	if(strncmp(sign, "RIFF", 4) != 0){
		Debug::showError(WRONG_SIGNATURE, (void*)path);
	}

	// Check the WAVE signature
	fseek(f, 4, SEEK_CUR);
	fread(sign, 4, sizeof(char), f);
	if(strncmp(sign, "WAVE", 4) != 0){
		Debug::showError(WRONG_SIGNATURE, (void*)path);
	}

	// Read the format header
	fseek(f, 4, SEEK_CUR);
	int a;
	fread(&a, sizeof(int), 1, f);
	if(a != 16){
		Debug::showError(UNSUPPORTED_FORMAT, (void*)path);			// Must be 16
	}
	a = 0;
	fread(&a, sizeof(unsigned short), 1, f);
	if(a != 1){
		Debug::showError(UNSUPPORTED_FORMAT, (void*)path);			// Must be 1
	}

	// Read the data header
	unsigned short nchannels;
	unsigned int samplerate;
	unsigned short bitspersample;
	fread(&nchannels, sizeof(unsigned short), 1, f);
	fread(&samplerate, sizeof(unsigned int), 1, f);
	fseek(f, 6, SEEK_CUR);
	fread(&bitspersample, sizeof(unsigned short), 1, f);
	fseek(f, 4, SEEK_CUR);		// Ignore "data" text

	// Read data
	unsigned int data_size;
	fread(&data_size, sizeof(unsigned int), 1, f);
	void *data = malloc (data_size);
	fread(data, data_size, 1, f);

	// Close the file
	fclose(f);

	// Create OpenAL buffers
	int format = AL_FORMAT_MONO8;
	if(nchannels == 2) format += 2;
	if(bitspersample == 16) format ++;
	alGenBuffers(1, &id);
	alBufferData(id, format, data, data_size, samplerate);
	free(data);
}

/**
 * @brief Destructor for a sound effect
 */
SFX::~SFX() {
	alDeleteBuffers(1, &id);
}

}
