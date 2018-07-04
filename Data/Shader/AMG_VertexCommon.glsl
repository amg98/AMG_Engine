uniform mat4 AMG_MVP;
out vec2 AMG_OutUV;									/**< Output texture coordinates */
uniform vec2 AMG_TexPosition;						/**< Texture position (for frame splitting) */
uniform vec2 AMG_TexScale;							/**< Texture scale (for frame splitting) */

/**
 * @brief Compute the final vertex position
 */
void AMG_ComputePosition(){
	gl_Position = AMG_MVP * vec4(AMG_Position, 1);
}

/**
 * @brief Compute the final vertex position, using a skinning matrix for animations
 */
void AMG_ComputePosition(in mat4 skinMatrix){
	gl_Position = AMG_MVP * skinMatrix * vec4(AMG_Position, 1);
}

/**
 * @brief Pass default data to the fragment shader, must be called
 */
void AMG_Pass(){
	AMG_OutUV = AMG_UV * AMG_TexScale + AMG_TexPosition;
}
