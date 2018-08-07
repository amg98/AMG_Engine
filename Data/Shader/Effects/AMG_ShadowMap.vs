#version 330 core

layout(location = 0) in vec3 AMG_Position;

#include <AMG_VertexCommon.glsl>

#include <AMG_ComputePosition.glsl>

void main(){

    AMG_ComputePosition();
}
