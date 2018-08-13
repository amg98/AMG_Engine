#version 330 core

layout (location = 0) out vec3 AMG_GPosition;
layout (location = 1) out vec3 AMG_GNormal;
layout (location = 2) out vec4 AMG_GAlbedo;

#include <AMG_FragmentCommon.glsl>

#include <AMG_ComputeLightCel.glsl>
#include <AMG_TextureMap.glsl>

in vec3 AMG_OutGPosition;
in vec3 AMG_OutGNormal;

void main(){

	//AMG_Color = AMG_ComputeLightCel(0, AMG_TextureMap(0));
	AMG_GPosition = AMG_OutGPosition;
	AMG_GNormal = normalize(AMG_OutGNormal);
	AMG_GAlbedo.rgb = texture(AMG_TextureSampler[0], AMG_OutUV).rgb;
	AMG_GAlbedo.a = 1.0;		// Specular map
}
