/**
 * @brief Pass deferred rendering data to the fragment shader
 * @param mv Modelview matrix
 * Output: AMG_OutGPositioon, AMG_OutGNormal
 * Input: AMG_Position, AMG_Normal
 */
void AMG_PassDeferred(mat4 mv){
	AMG_OutGPosition = (mv * vec4(AMG_Position, 1)).xyz;
	AMG_OutGNormal = (mv * vec4(AMG_Normal, 0)).xyz;
}
