/**
 * @file Music.h
 * @brief Loading and play back of music files in .ogg format
 */

#ifndef MUSIC_H_
#define MUSIC_H_

// OpenAL includes
#include <AL/al.h>

// Includes Vorbis OGG
#define OV_EXCLUDE_STATIC_CALLBACKS
#include <vorbis/vorbisfile.h>

// Own includes
#include "AudioSource.h"

// Defines
#define AMG_STREAM_BUFFERS 4	/**< Number of streaming buffers for OGG play back */

namespace AMG {

/**
 * @class Music
 * @brief Holds a OGG music file
 */
class Music: public AudioSource {
private:
	OggVorbis_File vorbisFile;			/**< Vorbis file handle */
	ALuint buffers[AMG_STREAM_BUFFERS];	/**< Decoding buffers */
	char *decodeBuffer;					/**< Decoding buffer in client side */
	unsigned int bufferSize;			/**< Decoding buffer size, in bytes */
	unsigned int channels;				/**< Number of channels */
	unsigned int frequency;				/**< Sample rate of the stream */
	unsigned int format;				/**< Internal format of the stream */
	bool playFlag;						/**< Playing flag */
public:
	Music(const char *path);
	void play(){ alSourcePlay(id); playFlag = true; }
	void update(double loop);
	virtual ~Music();
};

}

#endif
