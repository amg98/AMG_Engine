#version 330 core

const int AMG_TEXTURES = 2;

#include <AMG_FragmentCommon2D.glsl>
#include <AMG_FragmentTexture.glsl>

void main(){

    vec4 sceneColor = AMG_TextureMap(0);
	vec4 brightColor = AMG_TextureMap(1);
	AMG_Color = sceneColor + brightColor;
}
