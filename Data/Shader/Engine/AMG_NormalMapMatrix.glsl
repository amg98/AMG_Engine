/**
 * @brief Compute the normal map matrix for lighting
 * @param model Model matrix
 * @return The normal map matrix
 * Input: AMG_Normal, AMG_Tangent, AMG_Bitangent
 */
mat3 AMG_NormalMapMatrix(in mat4 model){
	vec3 n = normalize((model * vec4(AMG_Normal, 0)).xyz);
	vec3 t = normalize((model * vec4(AMG_Tangent, 0)).xyz);
	vec3 b = normalize((model * vec4(AMG_Bitangent, 0)).xyz);
	return mat3(t.x, b.x, n.x,
				t.y, b.y, n.y,
				t.z, b.z, n.z);
}
