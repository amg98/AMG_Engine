/**
 * @file AudioSource.cpp
 * @brief Creation of audio sources
 */

// Own includes
#include "AudioSource.h"

namespace AMG {

/**
 * @brief Constructor for an audio source
 * @param reference Distance with a gain of 1
 * @param end Maximum distance (more than that is a gain of 0)
 * @param rolloff How much the volume decreases
 */
AudioSource::AudioSource(float reference, float end, float rolloff) {
	alGenSources(1, &id);
	alSourcef(id, AL_ROLLOFF_FACTOR, rolloff);
	alSourcef(id, AL_REFERENCE_DISTANCE, reference);
	alSourcef(id, AL_MAX_DISTANCE, end);
}

/**
 * @brief Plays an audio file in this source
 * @param audio Audio file to play
 */
void AudioSource::play(SFX *audio){
	stop();
	alSourcei(id, AL_BUFFER, audio->getID());
	alSourcePlay(id);
}

/**
 * @brief Destructor for an audio source
 */
AudioSource::~AudioSource() {
	stop();
	alSourcei(id, AL_BUFFER, 0);
	alDeleteSources(1, &id);
}

}
