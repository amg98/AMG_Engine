#version 330 core

const int AMG_LIGHTS = 1;
const int AMG_TEXTURES = 1;
#include <AMG_FragmentUtility.glsl>

void main(){

	AMG_Color = AMG_ComputeLightDiffuse(0, AMG_TextureMap(0));
	AMG_ComputeFog();
}
