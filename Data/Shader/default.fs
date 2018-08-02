#version 330 core

layout (location = 0) out vec4 AMG_Color;

#include <AMG_FragmentCommon.glsl>

#include <AMG_ComputeLightCel.glsl>
#include <AMG_TextureMap.glsl>
#include <AMG_ComputeFog.glsl>

void main(){

	AMG_Color = AMG_ComputeLightCel(0, AMG_TextureMap(0));
	AMG_ComputeFog();
}
