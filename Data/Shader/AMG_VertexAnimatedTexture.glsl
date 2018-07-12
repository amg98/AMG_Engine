uniform vec4 AMG_TexPosition;						/**< Texture position for 2 frames */
uniform vec4 AMG_TexScale;							/**< Texture scale for 2 frames */

out vec2 AMG_OutUV[2];								/**< Output texture coordinates */

/**
 * @brief Pass texcoord data to the fragment shader
 */
void AMG_PassTexcoords(){
	AMG_OutUV[0] = AMG_UV * AMG_TexScale.xy + AMG_TexPosition.xy;
	AMG_OutUV[1] = AMG_UV * AMG_TexScale.zw + AMG_TexPosition.zw;
}
