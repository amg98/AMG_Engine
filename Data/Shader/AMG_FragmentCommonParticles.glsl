in float texblend;

const int AMG_TEXTURES = 1;

// Output data
out vec4 AMG_Color;							/**< Final color for this fragment */

// Uniforms
uniform sampler2D AMG_TextureSampler[AMG_TEXTURES];		/**< Texture samplers */
in vec2 AMG_OutUV[2];

vec4 AMG_ComputeParticlesColor(){
	vec4 c1 = texture(AMG_TextureSampler[0], AMG_OutUV[0]);
	vec4 c2 = texture(AMG_TextureSampler[0], AMG_OutUV[1]);
	
	return mix(c1, c2, texblend);
}
