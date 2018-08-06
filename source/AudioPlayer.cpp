/**
 * @file AudioPlayer.cpp
 * @brief Audio play back utilities and context creation for OpenAL applications
 */

// Includes C/C++
#include <stdio.h>

// Own includes
#include "Debug.h"
#include "AudioPlayer.h"

namespace AMG {

// Internal variables
ALCdevice *AudioPlayer::device = NULL;
ALCcontext *AudioPlayer::context = NULL;

/**
 * @brief Initialize the audio engine
 */
void AudioPlayer::initialize(){
	device = alcOpenDevice(NULL);
	if(device == NULL){
		Debug::showError(NO_AUDIO_DEVICE, NULL);
	}
	context = alcCreateContext(device, NULL);
	if(context == NULL){
		Debug::showError(NO_AUDIO_CONTEXT, NULL);
	}
	alcMakeContextCurrent(context);
}

/**
 * @brief Finish the audio engine
 */
void AudioPlayer::finish(){
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

/**
 * @brief Set listener properties
 * @param pos Listener's position
 * @param vel Listener's velocity
 */
void AudioPlayer::setListener(vec3 pos, vec3 vel){
	alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
	alListener3f(AL_VELOCITY, vel.x, vel.y, vel.z);
}

}
