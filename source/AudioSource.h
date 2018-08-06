/**
 * @file AudioSource.h
 * @brief Creation of audio sources
 */
#ifndef AUDIOSOURCE_H_
#define AUDIOSOURCE_H_

// Includes OpenAL
#include <AL/al.h>

// Includes OpenGL
#include <glm/glm.hpp>
using namespace glm;

// Own includes
#include "SFX.h"
#include "Entity.h"

namespace AMG {

/**
 * @class AudioSource
 * @brief Defines an audio source
 */
class AudioSource : public Entity {
protected:
	ALuint id;		/**< Source id */
public:
	AudioSource(float reference, float end, float rolloff);
	void setPosition(vec3 pos){ alSource3f(id, AL_POSITION, pos.x, pos.y, pos.z); }
	void setVolume(float gain){ alSourcef(id, AL_GAIN, gain); }
	void setVelocity(vec3 vel){ alSource3f(id, AL_VELOCITY, vel.x, vel.y, vel.z); }
	void setLoop(bool loop){ alSourcei(id, AL_LOOPING, loop); }
	bool playing(){ int state; alGetSourcei(id, AL_SOURCE_STATE, &state); return (state == AL_PLAYING); }
	void pause(){ alSourcePause(id); }
	void resume(){ alSourcePlay(id); }
	void stop(){ alSourceStop(id); }
	void setPitch(float pitch){ alSourcef(id, AL_PITCH, pitch); }
	void play(SFX *audio);
	virtual ~AudioSource();
};

}

#endif
