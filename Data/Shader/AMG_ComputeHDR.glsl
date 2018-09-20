/**
 * @brief Compute an HDR color to LDR
 * Uniforms: AMG_HDRExposure
 * Input: AMG_Color
 * Output: AMG_Color
 */
void AMG_ComputeHDR(){
	AMG_Color.rgb = (vec3(1.0) - exp(-AMG_Color.rgb * AMG_HDRExposure));
}
