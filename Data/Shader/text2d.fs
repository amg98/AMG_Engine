#version 330 core

layout (location = 0) out vec4 AMG_Color;

#include <AMG_FragmentCommon.glsl>

#include <AMG_ComputeTextColor.glsl>

void main(){

    AMG_ComputeTextColor();
}
