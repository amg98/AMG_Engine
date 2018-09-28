#version 330 core

layout(location = 0) in vec3 AMG_Position;
layout(location = 1) in vec2 AMG_UV;
layout(location = 2) in vec3 AMG_Normal;

#include <AMG_VertexCommon.glsl>

#include <AMG_PassTexcoordsReflection.glsl>
#include <AMG_WaterClipPlane.glsl>
#include <AMG_PassDeferred.glsl>
#include <AMG_ComputePosition.glsl>

void main(){

	AMG_WaterClipPlane(vec4(AMG_Position, 1));
    
    // Compute final vertex position
    AMG_ComputePosition();
    
    // Pass data to the fragment shader
	AMG_PassTexcoordsReflection();
	
	AMG_PassDeferred(AMG_MV);
}
