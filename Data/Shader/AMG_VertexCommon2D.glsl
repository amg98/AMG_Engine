// Input data layout
layout(location = 0) in vec2 AMG_Position;			/**< Vertex position, in model space */
layout(location = 1) in vec2 AMG_UV;				/**< Vertex texture coordinates */

// Output data
out vec2 AMG_OutUV;									/**< Output texture coordinates */

// 2D mode uniforms
uniform vec2 AMG_TexPosition;						/**< Texture position (for frame splitting) */
uniform vec2 AMG_TexScale;							/**< Texture scale (for frame splitting) */

// General uniforms
uniform mat4 AMG_MVP;								/**< Model-view-projection matrix */

/**
 * @brief Compute the final vertex position
 */
void AMG_ComputePosition(){
	gl_Position = AMG_MVP * vec4(AMG_Position, 0, 1);
}

/**
 * @brief Pass default data to the fragment shader, must be called (2D mode only)
 */
void AMG_Pass(){
	AMG_OutUV = AMG_UV * AMG_TexScale + AMG_TexPosition;
}
