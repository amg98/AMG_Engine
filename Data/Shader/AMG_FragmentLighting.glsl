in vec3 AMG_OutToLight[AMG_LIGHTS];
in float AMG_OutLightDistance[AMG_LIGHTS];
in vec3 AMG_OutNormal;
in vec3 AMG_OutToCamera;

const float AMG_CELSHADING_LEVELS = 5;

// Lighting uniforms
struct AMG_LightF {
	vec3 color;										/**< Light color */
	vec3 attenuation;								/**< Light attenuation: x*d^2 + y*d + z */
};
uniform AMG_LightF AMG_Lights[AMG_LIGHTS];			/**< Array holding all lights used in this shader */

// Material uniforms
uniform vec4 AMG_MaterialDiffuse;					/**< Material diffuse color */
uniform vec4 AMG_MaterialSpecular;					/**< Material specular color */
uniform vec4 AMG_MaterialAmbient;					/**< Material ambient color */
uniform float AMG_DiffusePower;						/**< Material diffuse power */
uniform float AMG_SpecularPower;					/**< Material specular power */

/**
 * @brief Compute a light's color (ambient + diffuse + specular + attenuation)
 * @param id The light's ID, must not surpass AMG_LIGHTS
 * @param diffuseTex Diffuse texture unit mapped
 */
vec4 AMG_ComputeLight(int id, vec4 diffuseTex){
	
	// Attenuation
	float attenuation = AMG_Lights[id].attenuation.x * AMG_OutLightDistance[id] * AMG_OutLightDistance[id] + 
						AMG_Lights[id].attenuation.y * AMG_OutLightDistance[id] +
						AMG_Lights[id].attenuation.z;
						
	// Diffuse
	float brightness = max(dot(AMG_OutNormal, AMG_OutToLight[id]), 0);
	vec4 diffuse = vec4(brightness * AMG_Lights[id].color / attenuation, 1) * AMG_MaterialDiffuse * AMG_DiffusePower;
    
	// Specular
	float specularBrightness = pow(max(dot(AMG_OutToCamera, reflect(-AMG_OutToLight[id], AMG_OutNormal)), 0), 10) * 0.8;
	vec4 specular = vec4(specularBrightness * AMG_Lights[id].color / attenuation, 1) * AMG_MaterialSpecular * AMG_SpecularPower;

    // Output color = color of the texture at the specified UV
    return (AMG_MaterialAmbient + diffuse) * diffuseTex + specular;
}
