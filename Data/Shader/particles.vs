#version 330 core

#include <AMG_VertexCommon2D.glsl>

void main(){

    AMG_ComputePosition();
    AMG_PassTexcoords();
}
