/**
 * @file Shader.h
 * @brief Description of shader loading and manipulation
 */

#ifndef SHADER_H_
#define SHADER_H_

// Includes C/C++
#include <string>
#include <tr1/unordered_map>
#include <vector>

// OpenGL includes
#include <GL/glew.h>
#include <glm/glm.hpp>
using namespace glm;

// Own includes
#include "Entity.h"
#include "Light.h"

// Defines
#define AMG_MAX_LIGHTS 4			/**< Maximum number of lights at once */
#define AMG_MAX_DLIGHTS 8			/**< Maximum number of lights in deferred rendering */
#define AMG_MAX_TEXTURES 6			/**< Maximum number of textures at once */
#define AMG_WATER_CLIPPING_PLANE 7	/**< Clipping plane for water rendering */

namespace AMG {

enum AMG_SHADER_UNIFORMS {
	AMG_MVP, AMG_BoneMatrix,
	AMG_NLights, AMG_MV, AMG_M,
	AMG_FogDensity, AMG_FogGradient, AMG_CamPosition, AMG_TexScale, AMG_TexPosition,
	AMG_ShadowMatrix, AMG_ShadowDistance, AMG_ShadowMapSize, AMG_ClippingPlanes,
	AMG_MaterialDiffuse, AMG_MaterialAmbient, AMG_MaterialSpecular, AMG_DiffusePower,
	AMG_SpecularPower, AMG_SprColor, AMG_FogColor, AMG_TexProgress, AMG_CharWidth,
	AMG_CharEdge, AMG_CharBorderWidth, AMG_CharBorderEdge, AMG_CharShadowOffset,
	AMG_CharOutlineColor, AMG_SSAOSamples, AMG_SSAOProjection, AMG_DView, AMG_HDRExposure,
	AMG_GammaValue, AMG_SpecularReflectivity, AMG_SSAOKernelSize, AMG_SSAOKernelRadius, AMG_WorldAmbient,
	AMG_RefractionIndex
};

/**
 * @class Shader
 * @brief Holds a shader program and its usage
 */
class Shader : private Entity {
private:
	int programID;											/**< Internal OpenGL program ID */
	std::tr1::unordered_map<std::string, int> uniformsMap;	/**< Hash map holding uniform variables in the shader */
	const static char *uniformsTable[];						/**< Uniforms table */
	int loadShader(const char *path, int type);
	std::string loadShaderCode(const char *path);
	void internalDefineUniform(std::string name);
public:
	int getProgram(){ return programID; }

	Shader(const char *file_path);
	void defineUniform(std::string name);
	int getUniform(const std::string &name);
	template<typename T> inline void setUniform(int id, T &v){ setUniform(uniformsTable[id], v); }
	void setUniform(const std::string &name, int v);
	void setUniform(const std::string &name, float v);
	void setUniform(const std::string &name, vec2 &v);
	void setUniform(const std::string &name, vec3 &v);
	void setUniform(const std::string &name, vec4 &v);
	void setUniform(const std::string &name, mat4 &v);
	void setUniform(const std::string &name, mat3 &v);
	void setUniform3fv(const std::string &name, int n, GLfloat *data);
	inline void setUniform3fv(int id, int n, GLfloat *data){ setUniform3fv(uniformsTable[id], n, data); }
	void setClipPlane(int id, vec4 &plane);
	inline void setWaterClipPlane(vec4 &plane){ setClipPlane(AMG_WATER_CLIPPING_PLANE, plane); }
	void disableClipPlane(int id);
	void disableWaterClipPlane(){ disableClipPlane(AMG_WATER_CLIPPING_PLANE); }
	void enable();
	inline void disable(){ glUseProgram(0); }
	virtual ~Shader();
};

}

#endif
