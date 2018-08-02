/**
 * @brief Pass texcoord data to the fragment shader
 * Input: AMG_UV
 * Output: AMG_OutUV
 */
void AMG_PassTexcoords(){
	AMG_OutUV = AMG_UV;
}
