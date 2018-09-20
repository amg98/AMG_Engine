#version 330 core

layout(location = 0) in vec3 AMG_Position;
layout(location = 1) in vec2 AMG_UV;
layout(location = 2) in vec3 AMG_Normal;

#include <AMG_VertexCommon.glsl>

#include <AMG_PassTexcoordsReflection.glsl>
#include <AMG_WaterClipPlane.glsl>

out vec3 AMG_OutGPosition;
out vec3 AMG_OutGNormal;

void main(){

	AMG_WaterClipPlane(vec4(AMG_Position, 1));
    
    // Compute final vertex position
    gl_Position = AMG_MVP * vec4(AMG_Position, 1);
    
    // Pass data to the fragment shader
	AMG_PassTexcoordsReflection();
	
	AMG_OutGPosition = (AMG_MV * vec4(AMG_Position, 1)).xyz;
	AMG_OutGNormal = (AMG_MV * vec4(AMG_Normal, 0)).xyz;
}
