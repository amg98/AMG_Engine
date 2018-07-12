#version 330 core

#include <AMG_InputSkybox.glsl>
#include <AMG_VertexCommon.glsl>
#include <AMG_VertexTextureCubeMap.glsl>

void main(){
    
	AMG_ComputePosition();
    
    AMG_PassTexcoords();
}
