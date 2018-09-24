/**
 * @brief Pass default data to the fragment shader, must be called
 * Input: AMG_Position
 * Output: AMG_OutUV3D
 */
void AMG_PassTexcoordsCubeMap(){
	AMG_OutUV3D = AMG_Position;
}
