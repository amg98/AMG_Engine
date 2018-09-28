#version 330 core

#include <AMG_FragmentCommon.glsl>

in vec2 blurUV[11];

void main(){

    AMG_Color = vec4(0.0);
	AMG_Color += texture(AMG_TextureSampler[0], blurUV[0]) * 0.0093;
    AMG_Color += texture(AMG_TextureSampler[0], blurUV[1]) * 0.028002;
    AMG_Color += texture(AMG_TextureSampler[0], blurUV[2]) * 0.065984;
    AMG_Color += texture(AMG_TextureSampler[0], blurUV[3]) * 0.121703;
    AMG_Color += texture(AMG_TextureSampler[0], blurUV[4]) * 0.175713;
    AMG_Color += texture(AMG_TextureSampler[0], blurUV[5]) * 0.198596;
    AMG_Color += texture(AMG_TextureSampler[0], blurUV[6]) * 0.175713;
    AMG_Color += texture(AMG_TextureSampler[0], blurUV[7]) * 0.121703;
    AMG_Color += texture(AMG_TextureSampler[0], blurUV[8]) * 0.065984;
    AMG_Color += texture(AMG_TextureSampler[0], blurUV[9]) * 0.028002;
    AMG_Color += texture(AMG_TextureSampler[0], blurUV[10]) * 0.0093;
}
