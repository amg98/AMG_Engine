/**
 * @file AudioPlayer.h
 * @brief Audio play back utilities and context creation for OpenAL applications
 */

#ifndef AUDIOPLAYER_H_
#define AUDIOPLAYER_H_

// OpenAL includes
#include <AL/al.h>
#include <AL/alc.h>

// OpenGL includes
#include <glm/glm.hpp>
using namespace glm;

namespace AMG {

/**
 * @class AudioPlayer
 * @brief Audio play back utilities and context creation for OpenAL applications
 */
class AudioPlayer {
private:
	static ALCdevice *device;		/**< Audio device */
	static ALCcontext *context;	/**< Audio context */
	AudioPlayer(){}
public:
	static void initialize();
	static void setListener(vec3 pos, vec3 vel);
	static void setDistanceModel(ALuint model){ alDistanceModel(model); }
	static void finish();
};

}

#endif
