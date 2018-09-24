/**
 * @brief Pass fog data to the fragment shader
 * @param modelview Modelview matrix
 * Uniforms: AMG_FogDensity, AMG_FogGradient
 * Input: AMG_Position
 * Output: AMG_OutFogValue
 */
void AMG_PassFog(in mat4 modelview){
	float distance = length((modelview * vec4(AMG_Position, 1)).xyz);
	AMG_OutFogValue = clamp(exp(-pow(AMG_FogDensity * distance, AMG_FogGradient)), 0, 1);
}
