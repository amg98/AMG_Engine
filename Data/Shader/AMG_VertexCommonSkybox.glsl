uniform mat4 AMG_MVP;
out vec3 AMG_OutUV;									/**< Output texture coordinates */
uniform vec2 AMG_TexPosition;						/**< Texture position (for frame splitting) */
uniform vec2 AMG_TexScale;							/**< Texture scale (for frame splitting) */

/**
 * @brief Compute the final vertex position
 */
void AMG_ComputePosition(){
	gl_Position = AMG_MVP * vec4(AMG_Position, 1);
}

/**
 * @brief Pass default data to the fragment shader, must be called
 */
void AMG_Pass(){
	AMG_OutUV = AMG_Position * vec3(AMG_TexScale, 1) + vec3(AMG_TexPosition, 0);
}
