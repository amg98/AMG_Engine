#version 330 core

layout (location = 0) out vec4 AMG_Color;

#include <AMG_FragmentCommon.glsl>

#include <AMG_ComputeLight.glsl>
#include <AMG_ComputeFog.glsl>
#include <AMG_TextureCubeMapReflection.glsl>

void main(){

	vec4 color = AMG_ComputeLight(0, vec4(1, 1, 1, 1));
	
	vec4 reflectedColor = AMG_TextureCubeMapReflection();
	AMG_Color = mix(color, reflectedColor, 0.6);
	
	AMG_ComputeFog();
}
