#version 330 core

layout(location = 0) in vec3 AMG_Position;
layout(location = 1) in vec2 AMG_UV;
layout(location = 2) in vec3 AMG_Normal;

#include <AMG_VertexCommon.glsl>

#include <AMG_ComputePosition.glsl>
#include <AMG_PassTexcoords.glsl>
#include <AMG_PassLighting.glsl>
#include <AMG_PassLight.glsl>
#include <AMG_WaterClipPlane.glsl>
#include <AMG_PassShadows.glsl>

out vec3 AMG_OutGPosition;
out vec3 AMG_OutGNormal;

void main(){

	AMG_WaterClipPlane(vec4(AMG_Position, 1));
	
	// Compute shadows
	vec3 outToCam = AMG_CamPosition - (AMG_M * vec4(AMG_Position, 1)).xyz;
	AMG_OutShadowCoord = 0.5 + 0.5 * AMG_ShadowMatrix * AMG_M * vec4(AMG_Position, 1.0);
	float d = length(outToCam);
	d = d - (AMG_ShadowDistance - 10.0);
	d = d / 10.0;
	AMG_OutShadowCoord.w = clamp(1.0 - d, 0.0, 1.0);
    
    // Compute final vertex position
    AMG_ComputePosition();
    
    // Pass data to the fragment shader
	AMG_PassTexcoords();
	
	AMG_OutGPosition = (AMG_MV * vec4(AMG_Position, 1)).xyz;
	AMG_OutGNormal = (AMG_MV * vec4(AMG_Normal, 0)).xyz;
}
