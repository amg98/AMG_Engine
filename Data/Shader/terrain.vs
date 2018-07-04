#version 330 core

const int AMG_LIGHTS = 1;

#include <AMG_InputAnimatedModel.glsl>

#include <AMG_VertexCommon.glsl>
#include <AMG_VertexLighting.glsl>
#include <AMG_VertexFog.glsl>

void main(){
    
    // Compute final vertex position
    AMG_ComputePosition();
    
    // Pass data to the fragment shader
    AMG_Pass();
	AMG_PassLighting(AMG_MV);
    AMG_PassLight(AMG_M, 0);
    AMG_PassFog(AMG_MV);
}
