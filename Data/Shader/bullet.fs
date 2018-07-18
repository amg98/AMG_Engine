#version 330 core

const int AMG_LIGHTS = 1;
const int AMG_TEXTURES = 1;

#include <AMG_FragmentCommon.glsl>

#include <AMG_FragmentTexture.glsl>
#include <AMG_FragmentLighting.glsl>
#include <AMG_FragmentFog.glsl>

void main(){

	AMG_Color = AMG_ComputeLight(0, vec4(1, 1, 1, 1));
	AMG_ComputeFog();
}
