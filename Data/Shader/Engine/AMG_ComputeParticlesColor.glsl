/**
 * @brief Compute the final color for the particles
 * Uniforms: AMG_TextureSampler
 * Input: AMG_OutUV2, AMG_TexBlend
 */
vec4 AMG_ComputeParticlesColor(){
	vec4 c1 = texture(AMG_TextureSampler[0], AMG_OutUV2[0]);
	vec4 c2 = texture(AMG_TextureSampler[0], AMG_OutUV2[1]);
	
	return mix(c1, c2, AMG_TexBlend);
}
