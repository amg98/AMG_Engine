/**
 * @file AMG_FragmentUtility.glsl
 * @brief GLSL functions defined for compatability with the Engine (fragment shader version)
 */

// Inputs from the vertex shader
in vec2 AMG_OutUV;
in vec3 AMG_OutNormal;
in vec3 AMG_OutToLight[AMG_LIGHTS];
in vec3 AMG_OutToCamera;
in float AMG_OutFogValue;

// Output data
out vec4 AMG_Color;							/**< Final color for this fragment */

// Uniforms
uniform sampler2D AMG_TextureSampler[AMG_TEXTURES];		/**< Texture samplers */

// Lighting uniforms
struct AMG_LightF {
	vec3 color;										/**< Light color */
};
uniform AMG_LightF AMG_Lights[AMG_LIGHTS];			/**< Array holding all lights used in this shader */

uniform vec4 AMG_FogColor;							/**< Fog color (and clear color) */

// Material uniforms
uniform vec4 AMG_MaterialDiffuse;					/**< Material diffuse color */
uniform vec4 AMG_MaterialSpecular;					/**< Material specular color */
uniform vec4 AMG_MaterialAmbient;					/**< Material ambient color */
uniform vec4 AMG_DiffusePower;						/**< Material diffuse power */
uniform vec4 AMG_SpecularPower;						/**< Material specular power */

/**
 * @brief Compute a light's diffuse color
 * @param id The light's ID, must not surpass AMG_LIGHTS
 * @param diffuseTex Diffuse texture unit mapped
 */
vec4 AMG_ComputeLightDiffuse(int id, vec4 diffuseTex){

	// Diffuse
	float brightness = max(dot(AMG_OutNormal, AMG_OutToLight[id]), 0);
	vec4 diffuse = vec4(brightness * AMG_Lights[id].color, 1) * AMG_MaterialDiffuse * AMG_DiffusePower;

    // Output color = color of the texture at the specified UV
    return (AMG_MaterialAmbient + diffuse) * diffuseTex;
}

/**
 * @brief Compute a light's color (diffuse + specular)
 * @param id The light's ID, must not surpass AMG_LIGHTS
 * @param diffuseTex Diffuse texture unit mapped
 */
vec4 AMG_ComputeLight(int id, vec4 diffuseTex){
	
	// Specular
	float specularBrightness = pow(max(dot(AMG_OutToCamera, reflect(-AMG_OutToLight[id], AMG_OutNormal)), 0), 10) * 0.8;
	vec4 specular = vec4(specularBrightness * AMG_Lights[id].color, 1) * AMG_MaterialSpecular * AMG_SpecularPower;

    // Output color = color of the texture at the specified UV
    return AMG_ComputeLightDiffuse(id, diffuseTex) + specular;
}

/**
 * @brief Compute fog in the final color, as a post-processing effect
 */
void AMG_ComputeFog(){
	AMG_Color = mix(AMG_FogColor, AMG_Color, AMG_OutFogValue);
}

/**
  * @brief Computes a texture mapping
  * @param tex Texture slot to use
  * @return The mapped color
  */
vec4 AMG_TextureMap(int tex){
	return texture(AMG_TextureSampler[tex], AMG_OutUV);
}

/**
 * @brief Perform a blending of textures using a blend map
 * @param t0 Red component of the blend map
 * @param t1 Green component of the blend map
 * @param t2 Blue component of the blend map
 * @param t3 The remaining color of the blend map
 * @param blendMap The blend map mask texture
 * @return The blended textures in this fragment
 */
vec4 AMG_BlendMap(int t0, int t1, int t2, int t3, int blendMap){
	vec4 blendMapColor = texture(AMG_TextureSampler[blendMap], AMG_OutUV);
	float backTextureAmount = 1 - (blendMapColor.r + blendMapColor.g + blendMapColor.b);
	vec2 tiledCoords = AMG_OutUV * 128;
	vec4 bgTexture = texture(AMG_TextureSampler[t3], tiledCoords) * backTextureAmount;
	vec4 rTexture = texture(AMG_TextureSampler[t0], tiledCoords) * blendMapColor.r;
	vec4 gTexture = texture(AMG_TextureSampler[t1], tiledCoords) * blendMapColor.g;
	vec4 bTexture = texture(AMG_TextureSampler[t2], tiledCoords) * blendMapColor.b;
	return bgTexture + rTexture + gTexture + bTexture;
}
