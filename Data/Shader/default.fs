#version 330 core

#include <AMG_FragmentUtility.glsl>

void main(){

	AMG_Color = AMG_ComputeLightDiffuse(0);
	AMG_ComputeFog();
}
