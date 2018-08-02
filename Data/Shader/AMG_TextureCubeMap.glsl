/**
  * @brief Computes a cube map texture mapping
  * @return The mapped color
  * Uniforms: AMG_TextureCubeSampler
  * Input: AMG_OutUV3D
  */
vec4 AMG_TextureCubeMap(){
	return texture(AMG_TextureCubeSampler, AMG_OutUV3D);
}
