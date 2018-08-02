#version 330 core

layout (location = 0) out vec4 AMG_Color;

#include <AMG_FragmentCommon.glsl>

#include <AMG_BlendMap.glsl>
#include <AMG_ComputeLight.glsl>
#include <AMG_ComputeFog.glsl>

void main(){

	AMG_Color = AMG_ComputeLight(0, AMG_BlendMap(1, 2, 3, 0, 4));
	AMG_ComputeFog();
}
