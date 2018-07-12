#version 330 core

const int AMG_TEXTURES = 1;

#include <AMG_FragmentCommon2D.glsl>
#include <AMG_FragmentTexture.glsl>
#include <AMG_FragmentText2D.glsl>

void main(){

    AMG_ComputeTextColor();
}
