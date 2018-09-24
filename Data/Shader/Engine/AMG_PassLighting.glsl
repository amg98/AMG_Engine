/**
 * @brief Pass general lighting information to the fragment shader
 * @param model Model matrix
 * Input: AMG_Position, AMG_Normal
 * Output: AMG_OutNormal, AMG_OutToCamera, AMG_CamPosition
 */
void AMG_PassLighting(in mat4 model){
	AMG_OutNormal = (model * vec4(AMG_Normal, 0)).xyz;
	AMG_OutToCamera = AMG_CamPosition - (model * vec4(AMG_Position, 1)).xyz;
}
