/**
 * @file AMG_VertexUtility.glsl
 * @brief GLSL functions defined for compatability with the Engine
 */

// Input data layout
layout(location = 0) in vec3 AMG_Position;			/**< Vertex position, in model space */
layout(location = 1) in vec2 AMG_UV;				/**< Vertex texture coordinates */
layout(location = 2) in vec3 AMG_Normal;			/**< Vertex outside normal vector */
layout(location = 3) in vec4 AMG_Weight;			/**< Vertex weight for 4 bones */
layout(location = 4) in ivec4 AMG_WeightBoneID;		/**< Bones used for this vertex weights */

// Output data
out vec2 AMG_OutUV;									/**< Output texture coordinates */
out vec3 AMG_OutNormal;								/**< Output normal vector */
out vec3 AMG_OutToLight[AMG_LIGHTS];				/**< Vector pointing to the light sources */
out vec3 AMG_OutToCamera;							/**< Vector pointing to the camera */
out float AMG_OutFogValue;							/**< Fog interpolation value for this vertex */

// Fog uniforms
uniform float AMG_FogDensity;						/**< Fog density */
uniform float AMG_FogGradient;						/**< Fog gradient */

// Skinning uniforms
uniform mat4 AMG_BoneMatrix[AMG_MAX_BONES];			/**< Bone matrices */

// Lighting uniforms
struct AMG_LightV {
	vec3 position;									/**< Light position */
};
uniform AMG_LightV AMG_Light[AMG_LIGHTS];			/**< Array holding all lights used in this shader */
uniform mat4 AMG_MV;								/**< Model-view matrix */

// General uniforms
uniform mat4 AMG_MVP;								/**< Model-view-projection matrix */

// Internal variables
float amg_distance;									/**< Distance from a vertex to the camera, used internally */

/**
 * @brief Compute the skinning matrix used for animation
 * @return The skinning matrix
 */
mat4 AMG_ComputeSkinning(){
	return AMG_BoneMatrix[AMG_WeightBoneID[0]] * AMG_Weight[0] +
		   AMG_BoneMatrix[AMG_WeightBoneID[1]] * AMG_Weight[1] +
		   AMG_BoneMatrix[AMG_WeightBoneID[2]] * AMG_Weight[2] +
		   AMG_BoneMatrix[AMG_WeightBoneID[3]] * AMG_Weight[3];
}

/**
 * @brief Compute the final vertex position
 */
void AMG_ComputePosition(){
	gl_Position = AMG_MVP * vec4(AMG_Position, 1);
}

/**
 * @brief Compute the final vertex position, using a skinning matrix for animations
 */
void AMG_ComputePosition(in mat4 skinMatrix){
	gl_Position = AMG_MVP * skinMatrix * vec4(AMG_Position, 1);
}

/**
 * @brief Pass fog data to the fragment shader
 * @note Call always after AMG_Pass()
 */
void AMG_PassFog(){
	AMG_OutFogValue = clamp(exp(-pow(AMG_FogDensity * amg_distance, AMG_FogGradient)), 0, 1);
}

/**
 * @brief Pass light information to the fragment shader
 * @param modelview Modelview + Skinning matrix
 * @param id Light ID, must not surpass AMG_LIGHTS
 */
void AMG_PassLight(in mat4 modelview, int id){
	AMG_OutToLight[id] = normalize(AMG_Light[id].position - (modelview * vec4(AMG_Position, 1)).xyz);
}

/**
 * @brief Pass default data to the fragment shader, must be called
 * @param modelview Modelview + Skinning matrix
 */
void AMG_Pass(in mat4 modelview){
	AMG_OutUV = AMG_UV;
	AMG_OutNormal = normalize((modelview * vec4(AMG_Normal, 0)).xyz);
	AMG_OutToCamera = -(modelview * vec4(AMG_Position, 1)).xyz;
	amg_distance = length(AMG_OutToCamera);
    AMG_OutToCamera /= amg_distance;
}
