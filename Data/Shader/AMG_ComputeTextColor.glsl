/**
 * @brief Compute the color for a text, applying its effects
 * Uniforms: AMG_TextureSampler, AMG_CharWidth, AMG_CharEdge, AMG_CharBorderWidth, AMG_CharBorderEdge, AMG_CharShadowOffset, AMG_CharOutlineColor, AMG_SprColor
 * Input: AMG_OutUV
 * Output: AMG_Color
 */
void AMG_ComputeTextColor(){
	float distance = 1 - texture(AMG_TextureSampler[0], AMG_OutUV).a;
	float alpha = 1 - smoothstep(AMG_CharWidth, AMG_CharWidth + AMG_CharEdge, distance);
	
	float distanceBorder = 1 - texture(AMG_TextureSampler[0], AMG_OutUV + AMG_CharShadowOffset).a;
	float alphaBorder = 1 - smoothstep(AMG_CharBorderWidth, AMG_CharBorderWidth + AMG_CharBorderEdge, distanceBorder);
	
	float overallAlpha = alpha + (1 - alpha) * alphaBorder;
	vec3 overallColor = mix(AMG_CharOutlineColor, AMG_SprColor.xyz, alpha / overallAlpha);
	
	AMG_Color = vec4(overallColor, AMG_SprColor.w * overallAlpha);
}
