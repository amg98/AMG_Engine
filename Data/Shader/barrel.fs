#version 330 core

layout (location = 0) out vec3 AMG_GPosition;
layout (location = 1) out vec3 AMG_GNormal;
layout (location = 2) out vec4 AMG_GAlbedo;

#include <AMG_FragmentCommon.glsl>

#include <AMG_ComputeLightNormalMap.glsl>
#include <AMG_TextureMap.glsl>
#include <AMG_ComputeDeferredNormalMap.glsl>

void main(){
	
	AMG_ComputeDeferredNormalMap(1, texture(AMG_TextureSampler[0], AMG_OutUV).rgb * AMG_MaterialDiffuse.rgb * AMG_DiffusePower, AMG_SpecularPower * AMG_SpecularReflectivity);
}
