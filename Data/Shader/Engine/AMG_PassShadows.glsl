/**
 * @brief Pass shadow information to the fragment shader
 * @note Call it AFTER AMG_PassLighing
 * Uniforms: AMG_ShadowDistance, AMG_ShadowMatrix
 * Input: AMG_Position, AMG_OutToCamera
 * Output: AMG_OutShadowCoord
 */
void AMG_PassShadows(){
	AMG_OutShadowCoord = 0.5 + 0.5 * AMG_ShadowMatrix * AMG_M * vec4(AMG_Position, 1.0);
	float d = length(AMG_OutToCamera);
	d = d - (AMG_ShadowDistance - 10.0);
	d = d / 10.0;
	AMG_OutShadowCoord.w = clamp(1.0 - d, 0.0, 1.0);
}
