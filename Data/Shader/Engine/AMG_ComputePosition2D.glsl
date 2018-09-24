/**
 * @brief Compute the final vertex position
 * Uniforms: AMG_MVP
 * Input: AMG_Position
 * Output: gl_Position
 */
void AMG_ComputePosition2D(){
	gl_Position = AMG_MVP * vec4(AMG_Position, 0, 1);
	gl_Position.z = -1.0;
	gl_Position.w = 1;
}
