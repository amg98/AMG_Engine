// Input data layout
layout(location = 0) in vec2 AMG_Position;			/**< Vertex position, in model space */
layout(location = 1) in vec2 AMG_UV;				/**< Vertex texture coordinates */

// General uniforms
uniform mat4 AMG_MVP;								/**< Model-view-projection matrix */

/**
 * @brief Compute the final vertex position
 */
void AMG_ComputePosition(){
	gl_Position = AMG_MVP * vec4(AMG_Position, 0, 1);
}
