/**
  * @brief Computes a cube map texture mapping (reflection + refraction)
  * @return The mapped color
  * Uniforms: AMG_TextureCubeSampler
  * Input: AMG_ReflectedVector, AMG_RefractedVector
  */
vec4 AMG_TextureCubeMapReflectionRefraction(){
	return mix(texture(AMG_TextureCubeSampler, reflect(AMG_RView, AMG_RNormal)), texture(AMG_TextureCubeSampler, refract(AMG_RView, AMG_RNormal, AMG_RefractionIndex)), 0.5);
}
