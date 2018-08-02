/**
 * @brief Sample a texture map
 * @param id The texture slot to use
 * @return The mapped color
 * Uniforms: AMG_TextureSampler
 * Input: AMG_OutUV
 */
vec4 AMG_TextureMap(int id){
	return texture(AMG_TextureSampler[id], AMG_OutUV);
}
