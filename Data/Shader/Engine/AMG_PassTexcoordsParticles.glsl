/**
 * @brief Pass texture coordinates for particle rendering
 * Uniforms: AMG_TexScale
 * Input: AMG_UV, AMG_InstancedTexPosition
 * Output: AMG_TexBlend, AMG_OutUV2
 */
void AMG_PassTexcoordsParticles(){
	AMG_TexBlend = AMG_InstancedTexProgress;
	AMG_OutUV2[0] = AMG_UV * AMG_TexScale + AMG_InstancedTexPosition.xy;
	AMG_OutUV2[1] = AMG_UV * AMG_TexScale + AMG_InstancedTexPosition.zw;
}
