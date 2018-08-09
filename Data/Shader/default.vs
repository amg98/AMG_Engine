#version 330 core

layout(location = 0) in vec3 AMG_Position;
layout(location = 1) in vec2 AMG_UV;
layout(location = 2) in vec3 AMG_Normal;
layout(location = 3) in vec4 AMG_Weight;
layout(location = 4) in ivec4 AMG_WeightBoneID;

#include <AMG_VertexCommon.glsl>

#include <AMG_ComputePositionSkin.glsl>
#include <AMG_ComputeSkinning.glsl>
#include <AMG_PassTexcoords.glsl>
#include <AMG_PassLighting.glsl>
#include <AMG_PassLight.glsl>
#include <AMG_WaterClipPlane.glsl>
#include <AMG_PassFog.glsl>

void main(){

	AMG_WaterClipPlane(vec4(AMG_Position, 1));
	
	// Compute skinning
	mat4 skin = AMG_ComputeSkinning();
	mat4 modelview = AMG_MV * skin;
	mat4 model = AMG_M * skin;
    
    // Compute final vertex position
    AMG_ComputePositionSkin(skin);
    
    // Pass data to the fragment shader
	AMG_PassTexcoords();
    AMG_PassLighting(model);
    AMG_PassLight(model, 0);
}
