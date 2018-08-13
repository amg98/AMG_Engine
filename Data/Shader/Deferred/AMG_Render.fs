#version 330 core

layout (location = 0) out vec4 AMG_Color;

#include <AMG_FragmentCommon.glsl>

void main(){

	FragPos = texture(AMG_TextureSampler[0], AMG_OutUV).rgb;
	vec3 Normal = texture(AMG_TextureSampler[1], AMG_OutUV).rgb;
	vec3 Albedo = texture(AMG_TextureSampler[2], AMG_OutUV).rgb;
	float Specular = texture(AMG_TextureSampler[2], AMG_OutUV).a;

    AMG_Color = vec4(texture(AMG_TextureSampler[0], AMG_OutUV).rgb, 1.0);
}
