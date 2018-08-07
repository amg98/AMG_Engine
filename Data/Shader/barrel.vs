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

void main(){
    
	AMG_WaterClipPlane(vec4(AMG_Position, 1));
	
    // Compute final vertex position
    AMG_ComputePosition();
    
    // Pass data to the fragment shader
	AMG_PassTexcoords();
	mat3 normalMapMatrix = AMG_NormalMapMatrix(AMG_M);
    AMG_PassLightingNormalMap(normalMapMatrix, AMG_M);
    AMG_PassLightNormalMap(normalMapMatrix, AMG_M, 0);
}
