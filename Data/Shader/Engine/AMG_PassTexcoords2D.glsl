/**
 * @brief Pass texcoord data to the fragment shader (2D mode)
 * Uniforms: AMG_TexPosition, AMG_TexScale
 * Input: AMG_UV
 * Output: AMG_OutUV2
 */
void AMG_PassTexcoords2D(){
	AMG_OutUV2[0] = AMG_UV * AMG_TexScale + AMG_TexPosition.xy;
	AMG_OutUV2[1] = AMG_UV * AMG_TexScale + AMG_TexPosition.zw;
}
