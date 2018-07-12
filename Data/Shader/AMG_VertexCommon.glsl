uniform mat4 AMG_MVP;

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
