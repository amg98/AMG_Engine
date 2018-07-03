#version 330 core

const int AMG_LIGHTS = 1;
const int AMG_TEXTURES = 5;
#include <AMG_FragmentUtility.glsl>

void main(){

	AMG_Color = AMG_ComputeLightDiffuse(0, AMG_BlendMap(1, 2, 3, 0, 4));
	AMG_ComputeFog();
}
