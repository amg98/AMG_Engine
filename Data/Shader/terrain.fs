#version 330 core

layout (location = 0) out vec3 AMG_GPosition;
layout (location = 1) out vec3 AMG_GNormal;
layout (location = 2) out vec4 AMG_GAlbedo;

#include <AMG_FragmentCommon.glsl>

#include <AMG_BlendMap.glsl>
#include <AMG_ComputeShadows.glsl>
#include <AMG_ComputeDeferred.glsl>

void main(){
	
	AMG_ComputeDeferred(AMG_BlendMap(1, 2, 3, 0, 4).rgb * AMG_ComputeShadows(5) * AMG_MaterialDiffuse.rgb * AMG_DiffusePower, AMG_SpecularPower * AMG_SpecularReflectivity);
}
