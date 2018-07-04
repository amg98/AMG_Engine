#version 330 core

const int AMG_MAX_BONES = 16;
const int AMG_LIGHTS = 1;

#include <AMG_InputAnimatedModel.glsl>

#include <AMG_VertexCommon.glsl>
#include <AMG_VertexSkinning.glsl>
#include <AMG_VertexLighting.glsl>
#include <AMG_VertexFog.glsl>

void main(){

	// Compute skinning
	mat4 skin = AMG_ComputeSkinning();
	mat4 modelview = AMG_MV * skin;
	mat4 model = AMG_M * skin;
    
    // Compute final vertex position
    AMG_ComputePosition(skin);
    
    // Pass data to the fragment shader
	AMG_Pass();
    AMG_PassLighting(modelview);
    AMG_PassLight(model, 0);
    AMG_PassFog(modelview);
}
