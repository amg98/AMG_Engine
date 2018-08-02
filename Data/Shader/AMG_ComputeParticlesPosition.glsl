/**
 * @brief Compute the particles position
 * Input: AMG_Position, AMG_InstancedMVP
 * Output: gl_Position
 */
void AMG_ComputeParticlesPosition(){
	gl_Position = AMG_InstancedMVP * vec4(AMG_Position, 0, 1);
}
