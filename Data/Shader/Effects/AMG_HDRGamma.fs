#version 330 core

layout (location=0) out vec4 AMG_Color;

#include <AMG_FragmentCommon.glsl>

#include <AMG_TextureMap.glsl>

void main(){

    vec3 color = AMG_TextureMap(0).rgb;
	color = (vec3(1.0) - exp(-color * AMG_HDRExposure));
	color = pow(color, vec3(1.0/AMG_GammaValue));
	AMG_Color = vec4(color, 1.0);
}
