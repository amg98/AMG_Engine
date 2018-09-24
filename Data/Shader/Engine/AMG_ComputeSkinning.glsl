/**
 * @brief Compute the skinning matrix used for animation
 * @return The skinning matrix
 * Uniforms: AMG_BoneMatrix
 * Input: AMG_Weight
 */
mat4 AMG_ComputeSkinning(){
	return AMG_BoneMatrix[AMG_WeightBoneID[0]] * AMG_Weight[0] +
		   AMG_BoneMatrix[AMG_WeightBoneID[1]] * AMG_Weight[1] +
		   AMG_BoneMatrix[AMG_WeightBoneID[2]] * AMG_Weight[2] +
		   AMG_BoneMatrix[AMG_WeightBoneID[3]] * AMG_Weight[3];
}
