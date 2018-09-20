#version 330 core

layout(location = 0) in vec3 AMG_Position;
layout(location = 1) in vec2 AMG_UV;
layout(location = 2) in vec3 AMG_Normal;
layout(location = 3) in vec3 AMG_Tangent;
layout(location = 4) in vec3 AMG_Bitangent;

#include <AMG_VertexCommon.glsl>

#include <AMG_ComputePosition.glsl>
#include <AMG_PassTexcoords.glsl>
#include <AMG_NormalMapMatrix.glsl>
#include <AMG_PassLightingNormalMap.glsl>
#include <AMG_PassLightNormalMap.glsl>
#include <AMG_WaterClipPlane.glsl>
#include <AMG_PassFog.glsl>

out vec3 AMG_OutGPosition;
out mat3 AMG_OutGNormalMatrix;

void main(){

	AMG_WaterClipPlane(vec4(AMG_Position, 1));
	
    // Compute final vertex position
    AMG_ComputePosition();
    
    // Pass data to the fragment shader
	AMG_PassTexcoords();
	
	AMG_OutGPosition = (AMG_MV * vec4(AMG_Position, 1)).xyz;
	AMG_OutGNormalMatrix = transpose(AMG_NormalMapMatrix(AMG_MV));
}
