out vec3 AMG_OutUV3D;									/**< Output texture coordinates */

/**
 * @brief Pass default data to the fragment shader, must be called
 */
void AMG_PassTexcoords(){
	AMG_OutUV3D = AMG_Position;
}
