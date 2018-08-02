#version 330 core

layout (location=0) out vec4 AMG_Color;

#include <AMG_FragmentCommon.glsl>

#include <AMG_TextureMap.glsl>

void main(){

    vec4 sceneColor = AMG_TextureMap(0);
	vec4 brightColor = AMG_TextureMap(1);
	AMG_Color = sceneColor + brightColor;
}
