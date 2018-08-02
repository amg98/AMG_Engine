#version 330 core

layout(location = 0) in vec2 AMG_Position;
layout(location = 1) in vec2 AMG_UV;
layout(location = 2) in mat4 AMG_InstancedMVP;
layout(location = 6) in vec4 AMG_InstancedTexPosition;
layout(location = 7) in float AMG_InstancedTexProgress;

#include <AMG_VertexCommon.glsl>

#include <AMG_ComputeParticlesPosition.glsl>
#include <AMG_PassTexcoordsParticles.glsl>

void main(){
	
	AMG_ComputeParticlesPosition();
	AMG_PassTexcoordsParticles();
}
