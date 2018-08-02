/**
  * @brief Computes a cube map texture mapping (reflection + refraction)
  * @return The mapped color
  * Uniforms: AMG_TextureCubeSampler
  * Input: AMG_ReflectedVector, AMG_RefractedVector
  */
vec4 AMG_TextureCubeMapReflectionRefraction(){
	return mix(texture(AMG_TextureCubeSampler, AMG_ReflectedVector), texture(AMG_TextureCubeSampler, AMG_RefractedVector), 0.5);
}
