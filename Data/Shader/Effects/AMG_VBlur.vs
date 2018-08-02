#version 330 core

layout(location = 0) in vec2 AMG_Position;
layout(location = 1) in vec2 AMG_UV;

#include <AMG_VertexCommon.glsl>

#include <AMG_ComputePosition2D.glsl>

out vec2 blurUV[11];
uniform float targetHeight;

void main(){

    AMG_ComputePosition2D();
	
	float pixelSize = 1.0 / targetHeight;
	
	for(int i=-5;i<=5;i++){
		blurUV[i+5] = AMG_UV + vec2(0, pixelSize * i);
	}
}
