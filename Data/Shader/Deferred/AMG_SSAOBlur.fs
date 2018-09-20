#version 330 core

layout (location = 0) out float AMG_Color;

#include <AMG_FragmentCommon.glsl>

void main(){
	vec2 texelSize = 1.0 / vec2(textureSize(AMG_TextureSampler[0], 0));
    float result = 0.0;
    for (int x = -2; x < 2; ++x) {
        for (int y = -2; y < 2; ++y) {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(AMG_TextureSampler[0], AMG_OutUV + offset).r;
        }
    }
    AMG_Color = result / (4.0 * 4.0);
}
