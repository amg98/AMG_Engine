#version 330 core

const int AMG_TEXTURES = 1;

#include <AMG_FragmentCommonSkybox.glsl>

#include <AMG_FragmentCubeMap.glsl>

void main(){

	AMG_Color = AMG_TextureMap(0);
}
