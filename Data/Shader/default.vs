#version 330 core

const int AMG_MAX_BONES = 16;
const int AMG_LIGHTS = 1;
#include <AMG_VertexUtility.glsl>

void main(){

	// Compute skinning
	mat4 skin = AMG_ComputeSkinning();
	mat4 modelview = AMG_MV * skin;
    
    // Compute final vertex position
    AMG_ComputePosition(skin);
    
    // Pass data to the fragment shader
    AMG_Pass(modelview);
    AMG_PassLight(modelview, 0);
    AMG_PassFog();
}
