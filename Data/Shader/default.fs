#version 330 core

const int AMG_LIGHTS = 1;
const int AMG_TEXTURES = 1;

#include <AMG_FragmentCommon.glsl>

#include <AMG_FragmentTexture.glsl>
#include <AMG_FragmentLighting.glsl>
#include <AMG_FragmentFog.glsl>

void main(){

	AMG_Color = AMG_ComputeLightCel(0, AMG_TextureMap(0));
	AMG_ComputeFog();
}
