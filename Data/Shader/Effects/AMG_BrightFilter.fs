#version 330 core

layout (location = 0) out vec4 AMG_Color;

#include <AMG_FragmentCommon.glsl>

#include <AMG_TextureMap.glsl>

void main(){

    vec4 color = AMG_TextureMap(0);
	float brightness = (color.r * 0.2126) + (color.g * 0.7152) + (color.b * 0.0722);
	AMG_Color = color * brightness;
}
