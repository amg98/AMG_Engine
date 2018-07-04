#version 330 core

#include <AMG_InputSkybox.glsl>

#include <AMG_VertexCommonSkybox.glsl>

void main(){
    
	AMG_ComputePosition();
    
    AMG_Pass();
}
