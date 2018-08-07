/**
 * @brief Set a clipping plane for water rendering
 * @param pos Vector holding the current vertex position
 * Uniforms: AMG_ClippingPlanes, AMG_M
 * Output: gl_ClipDistance[7]
 */
void AMG_WaterClipPlane(vec4 pos){
	gl_ClipDistance[7] = dot(AMG_M * pos, AMG_ClippingPlanes[7]); 
}
