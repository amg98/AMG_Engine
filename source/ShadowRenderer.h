/**
 * @file ShadowRenderer.h
 * @brief Utilities to render shadows
 */

#ifndef SHADOWRENDERER_H_
#define SHADOWRENDERER_H_

// Own includes
#include "Shader.h"
#include "Framebuffer.h"
#include "Renderer.h"

namespace AMG {

/**
 * @class ShadowRenderer
 * @brief Utilities to render shadows using shadow maps
 */
class ShadowRenderer {
private:
	static Shader *shadowMapShader;		/**< Shadow map rendering shader */
	static Framebuffer *shadowMap;		/**< Shadow map framebuffer */
	static vec3 min, max;				/**< Bounding box for the shadow map */
	static float nearWidth, nearHeight;	/**< Near clipping plane dimensions */
	static float farWidth, farHeight;	/**< Far clipping plane dimensions */
	static mat4 lightViewMatrix;		/**< View matrix, in light space */
	static mat4 projectionMatrix;		/**< Projection matrix (ortho) */
	static float shadowDistance;		/**< Shadow distance */
	static float shadowOffset;			/**< Shadow offset behind the frustum */
	ShadowRenderer(){}
	static void updateShadowBox();
	static void calculateFrustumVertices(mat4 &rotation, vec3 &forwardVector, vec3 &centerNear, vec3 &centerFar, vec4 *points);
	static vec4 calculateLightSpaceFrustumCorner(vec3 startPoint, vec3 direction, float width);
	static vec3 getCenter();
	static void updateLightViewMatrix(vec3 direction, vec3 center);
public:
	static void initialize(int dimensions, float offset, float distance);
	static void updateShadowMap(AMG_FunctionCallback render, Light *light);
	static void updateShadows(int slot);
	static void finish();
};

}

#endif
