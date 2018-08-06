/** @file Debug.h
  * @brief Debug and utility functions
  */

#ifndef DEBUG_H_
#define DEBUG_H_

namespace AMG {

/**
 * @enum ErrorCodes
 * @brief Available error codes
 */
enum ErrorCodes {
	NO_ERROR = 0,					/**< No error found */
	GLFW_INIT_ERROR,				/**< When GLFW can't setup */
	GLFW_WINDOW_ERROR,				/**< When a window can't initialise */
	GLEW_INIT_ERROR,				/**< When GLEW can't setup */
	SHADER_CREATION_ERROR,			/**< When a shader can't be created */
	FILE_NOT_FOUND,					/**< No found file */
	SHADER_COMPILATION,				/**< Error compiling shader */
	PROGRAM_CREATION_ERROR,			/**< Error creating shader program */
	SHADER_LINKING,					/**< Error linking shader */
	UNIFORM_NOT_FOUND,				/**< When an uniform variable is not found */
	WRONG_SIGNATURE,				/**< When a file has wrong signature */
	UNSUPPORTED_FORMAT,				/**< A file format of characteristic of a file format is not supported */
	NOT_POWER_OF_TWO_TEXTURE,		/**< When a texture has not power-of-two dimensions */
	TOO_MANY_BONES,					/**< When the bones limit is exceeded */
	UNSUPPORTED_CHARACTER,			/**< When a character is not supported by a font */
	FRAMEBUFFER_CREATION,			/**< When it fails creating a framebuffer object */
	NO_VERTEX_DATA,					/**< When accessing unexistent vertex data (in a MeshData) */
	NO_AUDIO_DEVICE,				/**< When an audio device is not supported */
	NO_AUDIO_CONTEXT,				/**< When an audio context can't be created */
};

/**
 * @class Debug
 * @brief Utility and debug functions
 */
class Debug {
public:
	static void showError(int code, void *param);
};

}

#endif
