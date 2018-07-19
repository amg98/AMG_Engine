#version 330 core

#include <AMG_VertexCommon2D.glsl>
#include <AMG_VertexTexture.glsl>

out vec2 blurUV[11];
uniform float targetWidth;

void main(){

    AMG_ComputePosition();
	
	float pixelSize = 1.0 / targetWidth;
	
	for(int i=-5;i<=5;i++){
		blurUV[i+5] = AMG_UV + vec2(pixelSize * i, 0);
	}
	
}
