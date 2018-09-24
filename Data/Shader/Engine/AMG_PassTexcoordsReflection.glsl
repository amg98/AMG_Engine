/**
 * @brief Pass reflection texcoords
 * Uniforms: AMG_M, AMG_CamPosition
 * Input: AMG_Position, AMG_Normal
 * Output: AMG_ReflectedVector
 */
void AMG_PassTexcoordsReflection(){
	vec3 pos = (AMG_M * vec4(AMG_Position, 1)).xyz;
	AMG_RNormal = mat3(transpose(inverse(AMG_M))) * AMG_Normal;
	AMG_RView = normalize(pos - AMG_CamPosition);
}
