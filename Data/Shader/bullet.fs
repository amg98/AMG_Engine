#version 330 core

const int AMG_LIGHTS = 1;
const int AMG_TEXTURES = 1;

#include <AMG_FragmentCommon.glsl>
#include <AMG_FragmentTextureCubeMap.glsl>
#include <AMG_FragmentLighting.glsl>
#include <AMG_FragmentFog.glsl>

in vec3 reflectedVector;
in vec3 refractedVector;

void main(){

	vec4 color = AMG_ComputeLight(0, vec4(1, 1, 1, 1));
	
	vec4 reflectedColor = AMG_TextureCubeMapReflectionRefraction();
	AMG_Color = mix(color, reflectedColor, 0.6);
	
	AMG_ComputeFog();
}
