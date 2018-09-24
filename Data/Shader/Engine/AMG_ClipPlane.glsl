/**
 * @brief Set a clipping plane
 * @param i ID of the clipping plane (0-7)
 * @param pos Vector holding the current vertex position
 * Uniforms: AMG_ClippingPlanes, AMG_M
 * Output: gl_ClipDistance
 */
void AMG_ClipPlane(int i, vec4 pos){
	gl_ClipDistance[i] = dot(AMG_M * pos, AMG_ClippingPlanes[i]); 
}
