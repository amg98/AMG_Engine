#version 330 core

layout (location=0) out vec4 AMG_Color;

#include <AMG_FragmentCommon.glsl>

#include <AMG_TextureMap.glsl>

void main(){

    vec4 color = AMG_TextureMap(0);
	color.rgb = (vec3(1.0) - exp(-color.rgb * AMG_HDRExposure));
	color.rgb = pow(color.rgb, vec3(1.0/AMG_GammaValue));
	AMG_Color = color;
}
