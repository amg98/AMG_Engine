#version 330 core

layout (location = 0) out vec4 AMG_Color;

#include <AMG_FragmentCommon.glsl>

#include <AMG_TextureCubeMap.glsl>
#include <AMG_ComputeHDR.glsl>

void main(){

	AMG_Color = AMG_TextureCubeMap();
	AMG_ComputeHDR();
}
