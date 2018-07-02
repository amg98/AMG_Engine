#version 330 core

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
