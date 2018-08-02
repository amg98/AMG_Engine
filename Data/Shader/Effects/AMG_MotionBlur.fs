#version 330 core

layout (location=0) out vec4 AMG_Color;

#include <AMG_FragmentCommon.glsl>

void main(){

	vec4 c0 = texture(AMG_TextureSampler[0], AMG_OutUV);
	vec4 c1 = texture(AMG_TextureSampler[1], AMG_OutUV);
	vec4 c2 = texture(AMG_TextureSampler[2], AMG_OutUV);
	vec4 c3 = texture(AMG_TextureSampler[3], AMG_OutUV);
    AMG_Color = (c0 + c1 + c2 + c3) / 4;
}
