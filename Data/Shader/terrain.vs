#version 330 core

#include <AMG_VertexUtility.glsl>

void main(){
    
    // Compute final vertex position
    AMG_ComputePosition();
    
    // Pass data to the fragment shader
    AMG_Pass(AMG_MV);
    AMG_OutUV *= 128;
    AMG_PassLight(AMG_MV, 0);
    AMG_PassFog();
}
