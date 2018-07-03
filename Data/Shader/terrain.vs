#version 330 core

const int AMG_MAX_BONES = 1;
const int AMG_LIGHTS = 1;
#include <AMG_VertexUtility.glsl>

void main(){
    
    // Compute final vertex position
    AMG_ComputePosition();
    
    // Pass data to the fragment shader
    AMG_Pass(AMG_MV);
    AMG_PassLight(AMG_MV, 0);
    AMG_PassFog();
}
