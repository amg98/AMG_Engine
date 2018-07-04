uniform vec4 AMG_FogColor;							/**< Fog color */

in float AMG_OutFogValue;

/**
 * @brief Compute fog in the final color, as a post-processing effect
 */
void AMG_ComputeFog(){
	AMG_Color = mix(AMG_FogColor, AMG_Color, AMG_OutFogValue);
}
