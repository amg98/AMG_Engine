uniform float AMG_FogDensity;						/**< Fog density */
uniform float AMG_FogGradient;						/**< Fog gradient */

out float AMG_OutFogValue;							/**< Fog interpolation value for this vertex */

/**
 * @brief Pass fog data to the fragment shader
 * @param modelview Modelview matrix
 */
void AMG_PassFog(in mat4 modelview){
	float distance = length((modelview * vec4(AMG_Position, 1)).xyz);
	AMG_OutFogValue = clamp(exp(-pow(AMG_FogDensity * distance, AMG_FogGradient)), 0, 1);
}
