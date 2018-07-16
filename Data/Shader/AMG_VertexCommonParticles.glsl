// Input data layout
layout(location = 0) in vec2 AMG_Position;			/**< Vertex position, in model space */
layout(location = 1) in vec2 AMG_UV;				/**< Vertex texture coordinates */
layout(location = 2) in mat4 AMG_InstancedMVP;
layout(location = 6) in vec4 AMG_InstancedTexPosition;
layout(location = 7) in float AMG_InstancedTexProgress;

uniform vec2 AMG_TexScale;							/**< Texture scale for 2 frames */
out vec2 AMG_OutUV[2];								/**< Output texture coordinates */

out float texblend;

void AMG_ComputeParticlesPosition(){
	gl_Position = AMG_InstancedMVP * vec4(AMG_Position, 0, 1);
}

void AMG_PassTexcoordsInstanced(){
	texblend = AMG_InstancedTexProgress;
	AMG_OutUV[0] = AMG_UV * AMG_TexScale + AMG_InstancedTexPosition.xy;
	AMG_OutUV[1] = AMG_UV * AMG_TexScale + AMG_InstancedTexPosition.zw;
}
