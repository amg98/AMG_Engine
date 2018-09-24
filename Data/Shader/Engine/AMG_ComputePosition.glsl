/**
 * @brief Compute the final vertex position
 * Uniforms: AMG_MVP
 * Input: AMG_Position
 */
void AMG_ComputePosition(){
	gl_Position = AMG_MVP * vec4(AMG_Position, 1);
}
