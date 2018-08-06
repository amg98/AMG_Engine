/**
 * @file SFX.h
 * @brief Defines a sound effect in the audio engine
 */

#ifndef SFX_H_
#define SFX_H_

// OpenAL includes
#include <AL/al.h>
#include <AL/alc.h>

// Own includes
#include "Entity.h"

namespace AMG {

/**
 * @class SFX
 * @brief Sound effect loading and play back in uncompressed .wav format
 */
class SFX: public Entity {
private:
	ALuint id;		/**< ID for the sound attachment */
public:
	ALuint getID(){ return id; }
	SFX(const char *path);
	virtual ~SFX();
};

}

#endif
