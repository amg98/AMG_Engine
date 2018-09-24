/**
 * @brief Compute shadow diffuse component
 * @param id Texture slot for the shadow map
 * @return The shadow diffuse factor, multiply it by the diffuse color
 * Uniforms: AMG_ShadowMapSize, AMG_TextureSampler
 * Input: AMG_OutShadowCoord
 */
float AMG_ComputeShadows(int id){
	float texelSize = 1.0 / AMG_ShadowMapSize;
	float total = 0.0;
	
	for(int x=-AMG_PCF_SIZE;x<=AMG_PCF_SIZE;x++){
		for(int y=-AMG_PCF_SIZE;y<=AMG_PCF_SIZE;y++){
			float objectNearestLight = texture(AMG_TextureSampler[id], AMG_OutShadowCoord.xy + vec2(x, y) *  texelSize).r;
			if(AMG_OutShadowCoord.z > objectNearestLight + 0.002){		// Prevents shadow acne
				total += 1.0;
			}
		}
	}
	
	total /= AMG_PCF_TEXELS;
	return (1.0 - (total - AMG_OutShadowCoord.w));
}
