struct AMG_LightV {
	vec3 position;									/**< Light position */
};
uniform AMG_LightV AMG_Light[AMG_LIGHTS];			/**< Array holding all lights used in this shader */
uniform mat4 AMG_MV;								/**< Model-view matrix */
uniform mat4 AMG_M;									/**< Model matrix */

out vec3 AMG_OutToLight[AMG_LIGHTS];				/**< Vector pointing to the light sources */
out float AMG_OutLightDistance[AMG_LIGHTS];			/**< Distance from the light to the vertex */
out vec3 AMG_OutNormal;								/**< Output normal vector */
out vec3 AMG_OutToCamera;							/**< Vector pointing to the camera */


/**
 * @brief Pass general lighting information to the fragment shader
 */
void AMG_PassLighting(in mat4 modelview){
	AMG_OutNormal = normalize((modelview * vec4(AMG_Normal, 0)).xyz);
	AMG_OutToCamera = -normalize((modelview * vec4(AMG_Position, 1)).xyz);
}

/**
 * @brief Pass light information to the fragment shader
 * @param modelview Model + Skinning matrix
 * @param id Light ID, must not surpass AMG_LIGHTS
 */
void AMG_PassLight(in mat4 model, int id){
	AMG_OutToLight[id] = AMG_Light[id].position - (model * vec4(AMG_Position, 1)).xyz;
	AMG_OutLightDistance[id] = length(AMG_OutToLight[id]);
	AMG_OutToLight[id] /= AMG_OutLightDistance[id];
}
