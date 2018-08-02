/**
  * @brief Computes a texture mapping (animated)
  * @param tex Texture slot to use
  * @return The mapped color
  * Uniforms: AMG_TextureSampler, AMG_TexProgress
  * Input: AMG_OutUV2
  */
vec4 AMG_AnimatedTextureMap(int tex){
	vec4 c1 = texture(AMG_TextureSampler[tex], AMG_OutUV2[0]);
	vec4 c2 = texture(AMG_TextureSampler[tex], AMG_OutUV2[1]);
	return mix(c1, c2, AMG_TexProgress);
}
