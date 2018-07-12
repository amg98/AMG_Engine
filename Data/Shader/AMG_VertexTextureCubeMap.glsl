out vec3 AMG_OutUV;									/**< Output texture coordinates */

/**
 * @brief Pass default data to the fragment shader, must be called
 */
void AMG_PassTexcoords(){
	AMG_OutUV = AMG_Position;
}
