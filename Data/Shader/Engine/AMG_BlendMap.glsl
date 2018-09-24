/**
 * @brief Perform a blending of textures using a blend map
 * @param t0 Red component of the blend map
 * @param t1 Green component of the blend map
 * @param t2 Blue component of the blend map
 * @param t3 The remaining color of the blend map
 * @param blendMap The blend map mask texture
 * @return The blended textures in this fragment
 * Uniforms: AMG_TextureSampler
 * Input: AMG_OutUV
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
