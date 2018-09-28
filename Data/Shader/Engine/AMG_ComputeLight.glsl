/**
 * @brief Compute a light's contribution to the scene
 * @param id ID of the light
 * @param diffuseTex Texture color
 * Uniforms: AMG_Lights, AMG_MaterialAmbient, AMG_MaterialDiffuse, AMG_DiffusePower, AMG_MaterialSpecular, AMG_SpecularPower
 * Input: AMG_OutLightDistance, AMG_OutNormal, AMG_OutToLight, AMG_OutToCamera
 */
vec4 AMG_ComputeLight(int id, vec4 diffuseTex){
	
	vec3 n = normalize(AMG_OutNormal);
	vec3 toLight = normalize(AMG_OutToLight[id]);
	vec3 toCam = normalize(AMG_OutToCamera);
	
	vec3 spotDirection = AMG_Lights[id].spotDirection;
	if(spotDirection != vec3(0, 0, 0)){
		spotDirection = normalize(spotDirection);
	}
	if(dot(toLight, spotDirection) < AMG_Lights[id].spotCutoff)
		return vec4(0, 0, 0, 1);
		
	// Attenuation
	float attenuation = AMG_Lights[id].attenuation.x * AMG_OutLightDistance[id] * AMG_OutLightDistance[id] + 
						AMG_Lights[id].attenuation.y * AMG_OutLightDistance[id] +
						AMG_Lights[id].attenuation.z;
						
	// Diffuse
	float brightness = max(dot(n, toLight), AMG_MaterialAmbient);
	vec4 diffuse = vec4(brightness * AMG_Lights[id].color / attenuation, 1) * AMG_MaterialDiffuse * AMG_DiffusePower;
    
	// Specular
	float specularBrightness = pow(max(dot(toCam, reflect(-toLight, n)), 0), 10) * AMG_SpecularReflectivity;
	vec4 specular = vec4(specularBrightness * AMG_Lights[id].color / attenuation, 1) * AMG_MaterialSpecular * AMG_SpecularPower;

    // Output color = color of the texture at the specified UV
    vec4 r = (diffuse * diffuseTex + specular);
	r.a = clamp(r.a, 0.0, 1.0);
	return r;
}
