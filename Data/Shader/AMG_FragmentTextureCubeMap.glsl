in vec3 AMG_OutUV;

// Uniforms
uniform samplerCube AMG_TextureSampler[AMG_TEXTURES];		/**< Texture samplers */

/**
  * @brief Computes a texture mapping
  * @param tex Texture slot to use
  * @return The mapped color
  */
vec4 AMG_TextureMap(int tex){
	return texture(AMG_TextureSampler[tex], AMG_OutUV);
}
