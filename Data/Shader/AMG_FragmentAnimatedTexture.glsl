// Uniforms
uniform sampler2D AMG_TextureSampler[AMG_TEXTURES];		/**< Texture samplers */
uniform float AMG_TexProgress;							/**< Amount between frames */

in vec2 AMG_OutUV[2];

/**
  * @brief Computes a texture mapping
  * @param tex Texture slot to use
  * @return The mapped color
  */
vec4 AMG_TextureMap(int tex){
	vec4 c1 = texture(AMG_TextureSampler[tex], AMG_OutUV[0]);
	vec4 c2 = texture(AMG_TextureSampler[tex], AMG_OutUV[1]);
	return mix(c1, c2, AMG_TexProgress);
}
