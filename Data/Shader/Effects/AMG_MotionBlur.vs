#version 330 core

#include <AMG_VertexCommon2D.glsl>
#include <AMG_VertexTexture.glsl>

void main(){

    AMG_ComputePosition();
	AMG_PassTexcoords();
}
