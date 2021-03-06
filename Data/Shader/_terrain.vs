#version 330 core

layout(location = 0) in vec3 AMG_Position;
layout(location = 1) in vec2 AMG_UV;
layout(location = 2) in vec3 AMG_Normal;

#include <AMG_VertexCommon.glsl>

#include <AMG_ComputePosition.glsl>
#include <AMG_PassTexcoords.glsl>
#include <AMG_PassLighting.glsl>
#include <AMG_PassLight.glsl>
#include <AMG_PassShadows.glsl>
#include <AMG_WaterClipPlane.glsl>
#include <AMG_PassFog.glsl>

void main(){
	
    // Compute final vertex position
    AMG_ComputePosition();
	
    // Pass data to the fragment shader
    AMG_PassTexcoords();
	AMG_PassLighting(AMG_M);
    AMG_PassLight(AMG_M, 0);
	AMG_PassShadows();
}
