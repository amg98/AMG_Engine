/**
  * @brief Computes a cube map texture mapping (refraction)
  * @return The mapped color
  * Uniforms: AMG_TextureCubeSampler
  * Input: AMG_RefractedVector
  */
vec4 AMG_TextureCubeMapRefraction(){
	return texture(AMG_TextureCubeSampler, AMG_RefractedVector);
}
