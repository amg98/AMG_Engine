#version 330 core

layout (location = 0) out vec4 AMG_Color;

#include <AMG_FragmentCommon.glsl>
#include <AMG_ComputeHDR.glsl>

const int AMG_DLIGHTS = 8;

struct AMG_LightD {
	vec3 position;
	vec3 color;
};
uniform AMG_LightD AMG_LightDR[AMG_DLIGHTS];
uniform int AMG_NLights;
uniform mat3 AMG_DView;

void main(){

	// OPTIMIZE
	vec3 FragPos = texture(AMG_TextureSampler[0], AMG_OutUV).rgb;
	vec3 Normal = texture(AMG_TextureSampler[1], AMG_OutUV).rgb;
	vec3 Albedo = texture(AMG_TextureSampler[2], AMG_OutUV).rgb;
	float Specular = texture(AMG_TextureSampler[2], AMG_OutUV).a;
	float Ambient = texture(AMG_TextureSampler[3], AMG_OutUV).r;
	
	// TODO specular lighting, different types of light
	vec3 lighting = Albedo * Ambient * 0.2;
	vec3 viewDir = normalize(-FragPos);
	for(int i = 0; i < AMG_NLights; i++){
		vec3 lightDir = normalize(AMG_DView * AMG_LightDR[i].position - FragPos);
		vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * AMG_LightDR[i].color;
		lighting += diffuse;
	}
    
	AMG_Color = vec4(lighting, 1.0);
	AMG_ComputeHDR();
}
