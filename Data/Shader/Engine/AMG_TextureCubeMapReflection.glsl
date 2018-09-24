/**
  * @brief Computes a cube map texture mapping (reflection)
  * @return The mapped color
  * Uniforms: AMG_TextureCubeSampler
  * Input: AMG_ReflectedVector
  */
vec4 AMG_TextureCubeMapReflection(){
	return texture(AMG_TextureCubeSampler, reflect(AMG_RView, AMG_RNormal));
}
