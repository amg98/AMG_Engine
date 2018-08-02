/**
 * @brief Compute the final vertex position, using a skinning matrix for animations
 * @param skinMatrix Skinning matrix
 * Uniforms: AMG_MVP
 * Input: AMG_Position
 */
void AMG_ComputePositionSkin(in mat4 skinMatrix){
	gl_Position = AMG_MVP * skinMatrix * vec4(AMG_Position, 1);
}
