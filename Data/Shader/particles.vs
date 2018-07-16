#version 330 core

#include <AMG_VertexCommonParticles.glsl>

void main(){
	
	AMG_ComputeParticlesPosition();
	AMG_PassTexcoordsInstanced();
}
