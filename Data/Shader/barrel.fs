#version 330 core

layout (location = 0) out vec3 AMG_GPosition;
layout (location = 1) out vec3 AMG_GNormal;
layout (location = 2) out vec4 AMG_GAlbedo;

#include <AMG_FragmentCommon.glsl>

#include <AMG_ComputeLightNormalMap.glsl>
#include <AMG_TextureMap.glsl>

in vec3 AMG_OutGPosition;
in mat3 AMG_OutGNormalMatrix;

void main(){

	vec4 normalColor = 2 * texture(AMG_TextureSampler[1], AMG_OutUV) - 1;

	AMG_GPosition = AMG_OutGPosition;
	AMG_GNormal.xyz = normalize(AMG_OutGNormalMatrix * normalColor.rgb);
	AMG_GAlbedo.rgb = texture(AMG_TextureSampler[0], AMG_OutUV).rgb;
	AMG_GAlbedo.a = 1.0;		// Specular map
}
