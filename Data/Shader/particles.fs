#version 330 core

layout (location = 0) out vec4 AMG_Color;

#include <AMG_FragmentCommon.glsl>

#include <AMG_ComputeParticlesColor.glsl>

void main(){
	
	AMG_Color = AMG_ComputeParticlesColor();
}
