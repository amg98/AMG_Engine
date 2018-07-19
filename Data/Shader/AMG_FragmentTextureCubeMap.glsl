in vec3 AMG_OutUV3D;
in vec3 AMG_ReflectedVector;
in vec3 AMG_RefractedVector;

// Uniforms
uniform samplerCube AMG_TextureCubeSampler;		/**< Texture samplers */

/**
  * @brief Computes a texture mapping
  * @param tex Texture slot to use
  * @return The mapped color
  */
vec4 AMG_TextureCubeMap(){
	return texture(AMG_TextureCubeSampler, AMG_OutUV3D);
}

vec4 AMG_TextureCubeMapReflection(){
	return texture(AMG_TextureCubeSampler, AMG_ReflectedVector);
}

vec4 AMG_TextureCubeMapRefraction(){
	return texture(AMG_TextureCubeSampler, AMG_RefractedVector);
}

vec4 AMG_TextureCubeMapReflectionRefraction(){
	return mix(texture(AMG_TextureCubeSampler, AMG_ReflectedVector), texture(AMG_TextureCubeSampler, AMG_RefractedVector), 0.5);
}
