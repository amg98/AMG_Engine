/**
 * @file Music.cpp
 * @brief Loading and play back of music files in .ogg format
 */

// Includes C/C++
#include <stdio.h>
#include <stdlib.h>

// Own includes
#include "Music.h"
#include "Debug.h"

namespace AMG {

// Internal libvorbis callbacks
static ov_callbacks vorbis_callbacks = {
  (size_t (*)(void *, size_t, size_t, void *))  fread,
  (int (*)(void *, ogg_int64_t, int))           fseeko64,		// Don't use fseek
  (int (*)(void *))                             fclose,
  (long (*)(void *))                            ftell
};

/**
 * @brief Internal function to decode a block of the vorbis ogg file
 * @param psOggVorbisFile Vorbis file to read from
 * @param pDecodeBuffer Decode buffer, in client side
 * @param ulBufferSize Decoding buffer size, in bytes
 * @param ulChannels Number of channels of the stream
 * @return The read bytes
 */
static unsigned long DecodeOggVorbis(OggVorbis_File *psOggVorbisFile, char *pDecodeBuffer, unsigned long ulBufferSize, unsigned long ulChannels){

	// Variables
	int current_section;
	long lDecodeSize;
	unsigned long ulBytesDone = 0;
	bool read = true;

	// Read until filling the buffer
	while (read){
		lDecodeSize = ov_read(psOggVorbisFile, pDecodeBuffer + ulBytesDone, ulBufferSize - ulBytesDone, 0, 2, 1, &current_section);
		if (lDecodeSize > 0){
			ulBytesDone += lDecodeSize;
			if (ulBytesDone >= ulBufferSize)		// Exceeding the buffer?
				read = false;
		} else {									// End of stream?
			read = false;
		}
	}

	// Return the amount of bytes read
	return ulBytesDone;
}

/**
 * @brief Constructor for a Music file
 * @param path Path of the music file, only .ogg format
 */
Music::Music(const char *path) : AudioSource(0.0f, 1.0f, 0.0f){

	// Open the music file
	FILE *file = fopen(Entity::getFullPath(path, AMG_AUDIO), "rb");
	if(file == NULL){
		Debug::showError(FILE_NOT_FOUND, (void*)path);
	}

	// Start the vorbis handle
	ov_open_callbacks(file, &vorbisFile, NULL, 0, vorbis_callbacks);

	// Get information about the file
	vorbis_info *psVorbisInfo = ov_info(&vorbisFile, -1);
	if(psVorbisInfo){

		// Read sample rate and number of channels of the stream
		frequency = psVorbisInfo->rate;
		channels = psVorbisInfo->channels;
		format = 0;

		// Set the format
		if(psVorbisInfo->channels == 1){
			format = AL_FORMAT_MONO16;
			bufferSize = frequency >> 1;			// Set buffer size to 250ms
			bufferSize -= (bufferSize % 2);			// Align the buffer size
		} else if (psVorbisInfo->channels == 2){
			format = AL_FORMAT_STEREO16;
			bufferSize = frequency;
			bufferSize -= (bufferSize % 4);
		}

		// Check whether the format is supported
		if(format == 0){
			Debug::showError(UNSUPPORTED_FORMAT, (void*)path);
		}
	} else {
		Debug::showError(WRONG_SIGNATURE, (void*)path);
	}

	// Create the stream buffers
	decodeBuffer = (char*) malloc (bufferSize);
	alGenBuffers(AMG_STREAM_BUFFERS, buffers);

	// Fill the buffers
	unsigned int written = 0;
	for(int i=0;i<AMG_STREAM_BUFFERS;i++){
		written = DecodeOggVorbis(&vorbisFile, decodeBuffer, bufferSize, channels);
		if(written){
			alBufferData(buffers[i], format, decodeBuffer, written, frequency);
			alSourceQueueBuffers(id, 1, &buffers[i]);
		}
	}

	// Initialize variables
	playFlag = false;
}

/**
 * @brief Update a music play
 * @param loop Loop point of the song, in seconds (-1 if no loop is needed)
 * @note Don't call the setLoop method for music
 */
void Music::update(double loop){

	// Update only if playing
	if(!playFlag) return;

	// Get the number of processed buffers
	int processedBuffers = 0;
	alGetSourcei(id, AL_BUFFERS_PROCESSED, &processedBuffers);

	// For each processed buffer, remove it from the Source Queue, read next chunk of audio
	// data from disk, fill buffer with new data, and add it to the Source Queue
	ALuint buffer;
	ALuint written;
	while (processedBuffers){
		// Remove the Buffer from the Queue
		buffer = 0;
		alSourceUnqueueBuffers(id, 1, &buffer);

		// Read more audio data
		written = DecodeOggVorbis(&vorbisFile, decodeBuffer, bufferSize, channels);
		if (written){
			alBufferData(buffer, format, decodeBuffer, written, frequency);
			alSourceQueueBuffers(id, 1, &buffer);
		}

		processedBuffers --;
	}

	// Check whether the source is playing
	if(!playing()){
		int queuedBuffers;
		alGetSourcei(id, AL_BUFFERS_QUEUED, &queuedBuffers);
		if (queuedBuffers){					// Starved audio buffers, restart
			alSourcePlay(id);
		} else if(loop != -1){				// Loop the song
			ov_time_seek(&vorbisFile, 0);
			for(int i=0;i<AMG_STREAM_BUFFERS;i++){
				written = DecodeOggVorbis(&vorbisFile, decodeBuffer, bufferSize, channels);
				if(written){
					alBufferData(buffers[i], format, decodeBuffer, written, frequency);
					alSourceQueueBuffers(id, 1, &buffers[i]);
				}
			}
			alSourcePlay(id);
		} else {							// Not playing anymore
			playFlag = false;
		}
	}
}

/**
 * @brief Destructor for a music file
 */
Music::~Music() {
	alDeleteBuffers(AMG_STREAM_BUFFERS, buffers);
	ov_clear(&vorbisFile);
	if(decodeBuffer) free(decodeBuffer);
}

}
