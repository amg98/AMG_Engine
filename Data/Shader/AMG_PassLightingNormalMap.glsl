/**
 * @brief Pass general lighting information to the fragment shader (normal map mode)
 * @param normalMatrix Normal mapping matrix
 * @param model Model matrix
 * Input: AMG_Position, AMG_Normal
 * Output: AMG_OutToCamera, AMG_CamPosition
 */
void AMG_PassLightingNormalMap(in mat3 normalMatrix, in mat4 model){
	AMG_OutToCamera = normalMatrix * (AMG_CamPosition - (model * vec4(AMG_Position, 1)).xyz);
}
