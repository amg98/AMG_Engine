#version 330 core

layout (location = 0) out float AMG_Color;

#include <AMG_FragmentCommon.glsl>

uniform vec3 AMG_SSAOSamples[64];
uniform mat4 AMG_SSAOProjection;
uniform float AMG_SSAOKernelSize;
uniform float AMG_SSAOKernelRadius;

// tile noise texture over screen based on screen dimensions divided by noise size
const vec2 noiseScale = vec2(1440.0/4.0, 900.0/4.0);

void main(){

	// Fetch data from textures
    vec3 fragPos = texture(AMG_TextureSampler[0], AMG_OutUV).xyz;
	vec3 normal = texture(AMG_TextureSampler[1], AMG_OutUV).rgb;
	vec3 randomVec = texture(AMG_TextureSampler[2], AMG_OutUV * noiseScale).xyz;
	
	// Compute a tangent -> view space matrix
	vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN       = mat3(tangent, bitangent, normal);
	
	// Compute the occlusion factor
	float occlusion = 0.0;
	for(int i = 0; i < AMG_SSAOKernelSize; i++){
		// get sample position
		vec3 sample = TBN * AMG_SSAOSamples[i]; // From tangent to view-space
		sample = fragPos + sample * AMG_SSAOKernelRadius;

		vec4 offset = vec4(sample, 1.0);
		offset      = AMG_SSAOProjection * offset;    // from view to clip-space
		offset.xyz /= offset.w;               // perspective divide
		offset.xyz  = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
		
		float sampleDepth = texture(AMG_TextureSampler[0], offset.xy).z;
	
		float rangeCheck = smoothstep(0.0, 1.0, AMG_SSAOKernelRadius / abs(fragPos.z - sampleDepth));	// radius
		occlusion       += ((sampleDepth >= sample.z + 0.01) ? 1.0 : 0.0) * rangeCheck;
	}
	
	AMG_Color = (occlusion / AMG_SSAOKernelSize) * AMG_WorldAmbient;
}
