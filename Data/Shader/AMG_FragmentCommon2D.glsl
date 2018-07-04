// Inputs from the vertex shader
in vec2 AMG_OutUV;

// Output data
out vec4 AMG_Color;							/**< Final color for this fragment */

// Uniforms
uniform sampler2D AMG_TextureSampler;		/**< Texture sampler */
uniform vec4 AMG_SprColor;					/**< Sprite color */

/**
 * @brief Compute the final color
 */
void AMG_ComputeColor(){
	AMG_Color = AMG_SprColor * texture(AMG_TextureSampler, AMG_OutUV);
}
