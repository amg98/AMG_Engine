#version 330 core

const int AMG_TEXTURES = 1;

#include <AMG_VertexCommon2D.glsl>
#include <AMG_VertexTexture.glsl>

void main(){

    AMG_ComputePosition();
    AMG_PassTexcoords();
}
