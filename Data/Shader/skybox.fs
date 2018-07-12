#version 330 core

const int AMG_TEXTURES = 1;

#include <AMG_FragmentCommon.glsl>
#include <AMG_FragmentTextureCubeMap.glsl>

void main(){

	AMG_Color = AMG_TextureMap(0);
}
