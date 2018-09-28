/**
 * @brief Pass deferred rendering data to the fragment shader (using a normal map)
 * @param mv Modelview matrix
 * Output: AMG_OutGPositioon, AMG_OutGNormal
 * Input: AMG_Position, AMG_Normal
 */
void AMG_PassDeferredNormalMap(mat4 modelview){
	AMG_OutGPosition = (modelview * vec4(AMG_Position, 1)).xyz;
	AMG_OutGNormalMatrix = transpose(AMG_NormalMapMatrix(modelview));
}
