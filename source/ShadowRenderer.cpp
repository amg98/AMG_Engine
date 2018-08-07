/**
 * @file ShadowRenderer.cpp
 * @brief Utilities to render shadows
 */

// Includes C/C++
#include <math.h>

// Includes OpenGL
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Own includes
#include "ShadowRenderer.h"

namespace AMG {

// Static variables
Shader *ShadowRenderer::shadowMapShader = NULL;
Framebuffer *ShadowRenderer::shadowMap = NULL;
float ShadowRenderer::nearWidth;
float ShadowRenderer::nearHeight;
float ShadowRenderer::farWidth;
float ShadowRenderer::farHeight;
float ShadowRenderer::shadowDistance;
float ShadowRenderer::shadowOffset;
vec3 ShadowRenderer::min;
vec3 ShadowRenderer::max;
mat4 ShadowRenderer::lightViewMatrix;
mat4 ShadowRenderer::projectionMatrix;

/**
 * @brief Initialize the shadow rendering engine
 * @param dimensions Dimensions of the shadow map
 * @param offset Shadow bounding box offset from behind
 * @param distance Shadow rendering distance from the camera
 */
void ShadowRenderer::initialize(int dimensions, float offset, float distance){

	// Load the shader
	shadowMapShader = new Shader("Effects/AMG_ShadowMap.vs", "Effects/AMG_ShadowMap.fs");

	// Create the framebuffer
	shadowMap = new Framebuffer(dimensions, dimensions);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMap->getFbo());
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	shadowMap->createDepthTexture();

	// Initialize variables
	farWidth = (float) (distance * tanf(Renderer::getFOV()));
	nearWidth = (float) (0.1f * tanf(Renderer::getFOV()));
	farHeight = farWidth / Renderer::getAspectRatio();
	nearHeight = nearWidth / Renderer::getAspectRatio();
	lightViewMatrix = mat4(1.0f);
	projectionMatrix = mat4(1.0f);
	shadowDistance = distance;
	shadowOffset = offset;
}

/**
 * @brief Update the shadow map texture
 * @param render Rendering callback
 * @param light Light which casts shadows
 * @note It is highly recommended to draw the objects in the simplest way possible, and use 3D mode
 */
void ShadowRenderer::updateShadowMap(AMG_FunctionCallback render, Light *light){
	updateShadowBox();
	vec3 &lightPosition = light->getPosition();
	vec3 lightDirection = -lightPosition;
	projectionMatrix[0][0] = 2.0f / (max.x - min.x);
	projectionMatrix[1][1] = 2.0f / (max.y - min.y);
	projectionMatrix[2][2] = 2.0f / (min.z - max.z);
	updateLightViewMatrix(lightDirection, getCenter());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadowMap->getFbo());
	glViewport(0, 0, shadowMap->getWidth(), shadowMap->getHeight());
	glClear(GL_DEPTH_BUFFER_BIT);
	shadowMapShader->enable();
	Renderer::setProjection(&projectionMatrix);
	Renderer::setView(lightViewMatrix);
	render();
	shadowMap->unbind();
	Renderer::setPerspective();
	Renderer::setView(Renderer::getCamera()->getMatrix());
}

/**
 * @brief Update shadow information in the current shader
 * @param slot Slot for the texture map
 */
void ShadowRenderer::updateShadows(int slot){
	mat4 m = projectionMatrix * lightViewMatrix;
	Shader *shader = Renderer::getCurrentShader();
	shader->setUniform("AMG_ShadowMatrix", m);
	shader->setUniform("AMG_ShadowDistance", shadowDistance);
	shader->setUniform("AMG_ShadowMapSize", (float)shadowMap->getWidth());
	shadowMap->getDepthTexture()->bind(slot);
}

/**
 * @brief Update the light view matrix
 * @param direction Light's direction
 * @param center Bounding box center
 */
void ShadowRenderer::updateLightViewMatrix(vec3 direction, vec3 center){
	direction = glm::normalize(direction);
	float pitch = acosf(sqrtf(direction.x * direction.x + direction.z * direction.z));
	float yaw = atanf(direction.x / direction.z);
	yaw = direction.z > 0 ? yaw - M_PI : yaw;
	lightViewMatrix = glm::toMat4(quat(vec3(0, -yaw, 0)) * quat(vec3(pitch, 0, 0))) * glm::translate(mat4(1.0f), -center);
}

/**
 * @brief Update the shadow map bounding box
 */
void ShadowRenderer::updateShadowBox(){

	Camera *camera = Renderer::getCamera();
	mat4 rotation = camera->getRotationMatrix();
	vec3 &forwardVector = camera->getForward();

	vec3 toFar = forwardVector * shadowDistance;
	vec3 toNear = forwardVector * 0.1f;
	vec3 centerNear = toNear + camera->getPosition();
	vec3 centerFar = toFar + camera->getPosition();

	vec4 points[8];
	calculateFrustumVertices(rotation, forwardVector, centerNear, centerFar, points);

	bool first = true;
	for(int i=0;i<8;i++){
		vec4 &point = points[i];
		if (first) {
			min.x = point.x;
			max.x = point.x;
			min.y = point.y;
			max.y = point.y;
			min.z = point.z;
			max.z = point.z;
			first = false;
			continue;
		}
		if (point.x > max.x) {
	    	max.x = point.x;
		} else if (point.x < min.x) {
			min.x = point.x;
		}
		if (point.y > max.y) {
			max.y = point.y;
		} else if (point.y < min.y) {
			min.y = point.y;
		}
		if (point.z > max.z) {
			max.z = point.z;
		} else if (point.z < min.z) {
			min.z = point.z;
		}
	}

	max.z += shadowOffset;
}

/**
 * @brief Get the bounding box center in world space
 * @return The bounding box center
 */
vec3 ShadowRenderer::getCenter() {
	float x = (min.x + max.x) / 2.0f;
	float y = (min.y + max.y) / 2.0f;
	float z = (min.z + max.z) / 2.0f;
	vec4 cen = glm::inverse(lightViewMatrix) * vec4(x, y, z, 1);
	return vec3(cen.x, cen.y, cen.z);
}

/**
 * @brief Calculate the view frustum vertices
 * @param rotation Camera rotation matrix
 * @param forwardVector Camera forward vector
 * @param centerNear Near plane position, in world space
 * @param centerFar Far plane position, in world space
 * @param points Output calculated frustum vertices
 */
void ShadowRenderer::calculateFrustumVertices(mat4 &rotation, vec3 &forwardVector, vec3 &centerNear, vec3 &centerFar, vec4 *points){
	vec3 upVector = vec3(rotation[0][1], rotation[1][1], rotation[2][1]);
	vec3 rightVector = glm::cross(forwardVector, upVector);
	vec3 downVector = -upVector;
	vec3 leftVector = -rightVector;
	vec3 farTop = centerFar + upVector * farHeight;
	vec3 farBottom = centerFar + downVector * farHeight;
	vec3 nearTop = centerNear + upVector * nearHeight;
	vec3 nearBottom = centerNear + downVector * nearHeight;
	points[0] = calculateLightSpaceFrustumCorner(farTop, rightVector, farWidth);
	points[1] = calculateLightSpaceFrustumCorner(farTop, leftVector, farWidth);
	points[2] = calculateLightSpaceFrustumCorner(farBottom, rightVector, farWidth);
	points[3] = calculateLightSpaceFrustumCorner(farBottom, leftVector, farWidth);
	points[4] = calculateLightSpaceFrustumCorner(nearTop, rightVector, nearWidth);
	points[5] = calculateLightSpaceFrustumCorner(nearTop, leftVector, nearWidth);
	points[6] = calculateLightSpaceFrustumCorner(nearBottom, rightVector, nearWidth);
	points[7] = calculateLightSpaceFrustumCorner(nearBottom, leftVector, nearWidth);
}

/**
 * @brief Calculates one corner of the view frustum (in light space)
 * @param startPoint The starting center point on the view frustum
 * @param direction The direction of the corner from the start point
 * @param width The distance of the corner from the start point
 * @return The calculated corner of the frustum
 */
vec4 ShadowRenderer::calculateLightSpaceFrustumCorner(vec3 startPoint, vec3 direction, float width) {
	vec3 point = startPoint + direction * width;
	vec4 point4f = vec4(point.x, point.y, point.z, 1.0f);
	return lightViewMatrix * point4f;
}

/**
 * @brief Terminate the shadow rendering engine
 */
void ShadowRenderer::finish(){
	if(shadowMapShader) delete shadowMapShader;
	if(shadowMap) delete shadowMap;
}

}
