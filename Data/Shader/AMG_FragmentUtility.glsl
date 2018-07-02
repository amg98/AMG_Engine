/**
 * @file AMG_FragmentUtility.glsl
 * @brief GLSL functions defined for compatability with the Engine (fragment shader version)
 */

// Constants 
const int AMG_LIGHTS = 1;			/**< Must be the same as the vertex shader */

// Inputs from the vertex shader
in vec2 AMG_OutUV;
in vec3 AMG_OutNormal;
in vec3 AMG_OutToLight[AMG_LIGHTS];
in vec3 AMG_OutToCamera;
in float AMG_OutFogValue;

// Output data
out vec4 AMG_Color;							/**< Final color for this fragment */

// Uniforms
uniform sampler2D AMG_TextureSampler;		/**< Texture sampler */

// Lighting uniforms
struct AMG_LightF {
	vec3 color;										/**< Light color */
};
uniform AMG_LightF AMG_Lights[AMG_LIGHTS];			/**< Array holding all lights used in this shader */

uniform vec4 AMG_FogColor;							/**< Fog color (and clear color) */

/**
 * @brief Compute a light's diffuse color
 * @param id The light's ID, must not surpass AMG_LIGHTS
 */
vec4 AMG_ComputeLightDiffuse(int id){

	// Diffuse
	float brightness = max(dot(AMG_OutNormal, AMG_OutToLight[id]), 0);
	vec4 diffuse = vec4(brightness * AMG_Lights[id].color, 1);

    // Output color = color of the texture at the specified UV
    return diffuse * texture(AMG_TextureSampler, AMG_OutUV);
}

/**
 * @brief Compute a light's color (diffuse + specular)
 * @param id The light's ID, must not surpass AMG_LIGHTS
 */
vec4 AMG_ComputeLight(int id){
	
	// Specular
	float specularBrightness = pow(max(dot(AMG_OutToCamera, reflect(-AMG_OutToLight[id], AMG_OutNormal)), 0.2), 10) * 0.8;
	vec4 specular = vec4(specularBrightness * AMG_Lights[id].color, 1);

    // Output color = color of the texture at the specified UV
    return AMG_ComputeLightDiffuse(id) + specular;
}

/**
 * @brief Compute fog in the final color, as a post-processing effect
 */
void AMG_ComputeFog(){
	AMG_Color = mix(AMG_FogColor, AMG_Color, AMG_OutFogValue);
}
