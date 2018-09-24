/**
 * @brief Pass light information to the fragment shader (normal map mode)
 * @param normalMatrix Normal mapping matrix
 * @param model Model + Skinning matrix
 * @param id Light ID, must not surpass AMG_LIGHTS
 * Uniforms: AMG_Light
 * Input: AMG_Position
 * Output: AMG_OutToLight, AMG_OutLightDistance
 */
void AMG_PassLightNormalMap(in mat3 normalMatrix, in mat4 model, int id){
	AMG_OutToLight[id] = normalMatrix * (AMG_Light[id].position - (model * vec4(AMG_Position, 1)).xyz);
	AMG_OutLightDistance[id] = length(AMG_OutToLight[id]);
}
