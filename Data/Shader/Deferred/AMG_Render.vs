#version 330 core

layout(location = 0) in vec2 AMG_Position;
layout(location = 1) in vec2 AMG_UV;

#include <AMG_VertexCommon.glsl>

#include <AMG_ComputePosition2D.glsl>

void main(){

    AMG_ComputePosition2D();
    AMG_OutUV = AMG_UV;
}
