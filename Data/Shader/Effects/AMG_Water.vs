#version 330 core

layout (location = 0) in vec2 AMG_Position;

#include <AMG_VertexCommon.glsl>

out vec4 clipSpaceCoords;

void main(){
	clipSpaceCoords = AMG_MVP * vec4(AMG_Position.x, 0, AMG_Position.y, 1);
	gl_Position = clipSpaceCoords;
	AMG_OutUV = vec2(AMG_Position.x/2.0 + 0.5, AMG_Position.y/2.0 + 0.5) * 0.6;
	AMG_OutToCamera = AMG_CamPosition - (AMG_M * vec4(AMG_Position.x, 0, AMG_Position.y, 1)).xyz;
	AMG_OutToLight[0] = AMG_Light[0].position - (AMG_M * vec4(AMG_Position.x, 0, AMG_Position.y, 1)).xyz;
	
	float distance = length((AMG_MV * vec4(AMG_Position.x, 0, AMG_Position.y, 1)).xyz);
	AMG_OutFogValue = clamp(exp(-pow(AMG_FogDensity * distance, AMG_FogGradient)), 0, 1);
}
