#version 330 core

layout (location = 0) out vec4 AMG_Color;

#include <AMG_FragmentCommon.glsl>
#include <AMG_ComputeFog.glsl>

in vec4 clipSpaceCoords;

uniform float moveFactor;

void main(){

	vec2 ndc = (clipSpaceCoords.xy/clipSpaceCoords.w) / 2.0 + 0.5;
	vec3 cameraVector = normalize(AMG_OutToCamera);
	
	vec2 distortedTexCoords = texture(AMG_TextureSampler[2], vec2(AMG_OutUV.x + moveFactor, AMG_OutUV.y)).rg * 0.1;
	distortedTexCoords = AMG_OutUV + vec2(distortedTexCoords.x, distortedTexCoords.y + moveFactor);
	vec2 distortion = (texture(AMG_TextureSampler[2], distortedTexCoords).rg * 2.0 - 1.0) * 0.03;
	
	vec2 reflectionTex = vec2(ndc.x, 1.0 - ndc.y) + distortion;
	reflectionTex = clamp(reflectionTex, 0.001, 0.999);
	
	vec2 refractionTex = ndc.xy + distortion;
	refractionTex = clamp(refractionTex, 0.001, 0.999);
	
	vec4 reflectionColor = texture(AMG_TextureSampler[0], reflectionTex);
	vec4 refractionColor = texture(AMG_TextureSampler[1], refractionTex);
	
	vec4 normalMapColor = texture(AMG_TextureSampler[3], distortedTexCoords);
	vec3 normal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b, normalMapColor.g * 2.0 - 1.0);
	normal = normalize(normal);
	
	float refractivity = dot(cameraVector, normal);
	refractivity = clamp(pow(refractivity, 5.0), 0.0, 1.0);
	
	vec3 reflectedLight = reflect(normalize(-AMG_OutToLight[0]), normal);
	float specular = max(dot(reflectedLight, cameraVector), 0.0);
	specular = pow(specular, 20.0);
	vec3 specularHighlights = AMG_Lights[0].color * specular * 0.6;
	
	AMG_Color = mix(reflectionColor, refractionColor, refractivity);
	AMG_Color = mix(AMG_Color, vec4(0, 0.3, 0.5, 1.0), 0.2) + vec4(specularHighlights, 0.0);
}
