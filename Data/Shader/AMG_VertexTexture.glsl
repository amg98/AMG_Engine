out vec2 AMG_OutUV;									/**< Output texture coordinates */

/**
 * @brief Pass texcoord data to the fragment shader
 */
void AMG_PassTexcoords(){
	AMG_OutUV = AMG_UV;
}
