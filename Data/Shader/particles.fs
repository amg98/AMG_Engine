#version 330 core

#include <AMG_FragmentCommonParticles.glsl>

void main(){
	
	AMG_Color = AMG_ComputeParticlesColor();
}
