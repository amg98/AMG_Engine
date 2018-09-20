/**
 * @brief Perform a gamma correction in the final fragment color
 * Uniforms: AMG_GammaValue
 * Input: AMG_Color
 * Output: AMG_Color
 * @note You need to convert your diffuse textures to linear space (powering the luminance to 2.2) when using this effect
 */
void AMG_GammaCorrection(){
	AMG_Color.rgb = pow(AMG_Color.rgb, vec3(1.0/AMG_GammaValue));
}
