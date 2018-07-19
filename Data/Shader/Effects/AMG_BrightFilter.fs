#version 330 core

const int AMG_TEXTURES = 1;

#include <AMG_FragmentCommon2D.glsl>
#include <AMG_FragmentTexture.glsl>

void main(){

    vec4 color = AMG_TextureMap(0);
	float brightness = (color.r * 0.2126) + (color.g * 0.7152) + (color.b * 0.0722);
	AMG_Color = color * brightness;
}
