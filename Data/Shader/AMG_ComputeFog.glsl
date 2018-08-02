/**
 * @brief Compute fog in the final color, as a post-processing effect
 * Uniforms: AMG_FogColor
 * Input: AMG_OutFogValue
 * Output: AMG_Color
 */
void AMG_ComputeFog(){
	AMG_Color = mix(AMG_FogColor, AMG_Color, AMG_OutFogValue);
}
