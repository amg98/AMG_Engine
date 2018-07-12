#version 330 core

const int AMG_LIGHTS = 1;
const int AMG_TEXTURES = 5;

#include <AMG_FragmentCommon.glsl>
#include <AMG_FragmentTexture.glsl>
#include <AMG_FragmentLighting.glsl>
#include <AMG_FragmentFog.glsl>

void main(){

	AMG_Color = AMG_ComputeLight(0, AMG_BlendMap(1, 2, 3, 0, 4));
	AMG_ComputeFog();
}
