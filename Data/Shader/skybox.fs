#version 330 core

#include <AMG_FragmentCommon.glsl>
#include <AMG_FragmentTextureCubeMap.glsl>

void main(){

	AMG_Color = AMG_TextureCubeMap();
}
