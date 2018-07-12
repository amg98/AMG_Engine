#version 330 core

const int AMG_TEXTURES = 1;

#include <AMG_FragmentCommon2D.glsl>
#include <AMG_FragmentAnimatedTexture.glsl>

void main(){

    AMG_Color = AMG_SprColor * AMG_TextureMap(0);
}
