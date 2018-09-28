#version 330 core

layout (location = 0) out vec4 AMG_Color;

#include <AMG_FragmentCommon.glsl>

uniform int AMG_NLights;
uniform mat4 AMG_DView;

uniform float AMG_FogDensity;
uniform float AMG_FogGradient;

void main(){

	// Get color values from the G-buffer
	vec3 FragPos = texture(AMG_TextureSampler[0], AMG_OutUV).rgb;
	vec3 Normal = texture(AMG_TextureSampler[1], AMG_OutUV).rgb;
	vec4 t2 = texture(AMG_TextureSampler[2], AMG_OutUV);
	vec3 Albedo = t2.rgb;
	float Specular = t2.a;
	float Ambient = 1.0 - texture(AMG_TextureSampler[3], AMG_OutUV).r;
	
	// Compute lighting
	vec3 lighting = vec3(0.0);
	vec3 viewDir = normalize(-FragPos);		// toCam
	for(int i = 0; i < AMG_NLights; i++){
		vec3 lightDir = (AMG_DView * vec4(AMG_LightDR[i].position, 1)).xyz - FragPos;		// toLight
		
		// Spotlights
		vec3 spotDirection = (AMG_DView * vec4(AMG_LightDR[i].spotDirection, 0)).xyz;
		if(spotDirection != vec3(0, 0, 0)){
			spotDirection = normalize(spotDirection);
		}
		float lightDistance = length(lightDir);
		lightDir = lightDir / lightDistance;
		if(dot(lightDir, spotDirection) > AMG_LightDR[i].spotCutoff){
		
			// Compute diffuse + specular
			float attenuation = AMG_LightDR[i].attenuation.x * lightDistance * lightDistance + 
								AMG_LightDR[i].attenuation.y * lightDistance +
								AMG_LightDR[i].attenuation.z;
			vec3 diffuse = max(dot(Normal, lightDir), Ambient) * Albedo * AMG_LightDR[i].color / attenuation;
			vec3 specular = AMG_LightDR[i].color / attenuation * pow(max(dot(viewDir, reflect(-lightDir, Normal)), 0), 10) * Specular;
			lighting += (diffuse + specular);
		}
	}
    
	AMG_Color = vec4(lighting, 1.0);
	
	// Compute fog
	AMG_Color = mix(AMG_FogColor, AMG_Color, clamp(exp(-pow(AMG_FogDensity * length(FragPos), AMG_FogGradient)), 0, 1));
}
