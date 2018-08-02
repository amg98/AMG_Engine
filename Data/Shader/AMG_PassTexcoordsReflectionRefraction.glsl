/**
 * @brief Pass refraction texcoords
 * Uniforms: AMG_M, AMG_CamPosition
 * Input: AMG_Position, AMG_Normal
 * Output: AMG_ReflectedVector, AMG_RefractedVector
 */
void AMG_PassTexcoordsReflectionRefraction(){
	vec3 pos = (AMG_M * vec4(AMG_Position, 1)).xyz;
	vec3 normal = mat3(transpose(inverse(AMG_M))) * AMG_Normal;
	vec3 viewVector = normalize(pos - AMG_CamPosition);
	AMG_ReflectedVector = reflect(viewVector, normal);
    AMG_RefractedVector = refract(viewVector, normal, 1.0/1.33);
}
