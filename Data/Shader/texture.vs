#version 330 core

const int MAX_BONES = 16;

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec4 vertexWeight;
layout(location = 4) in ivec4 vertexWeightBoneID;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

// Values that stay constant for the whole mesh.
uniform mat4 boneMatrix[MAX_BONES];
uniform mat4 MVP;

void main(){

	mat4 skin = boneMatrix[vertexWeightBoneID[0]] * vertexWeight[0] +
			   boneMatrix[vertexWeightBoneID[1]] * vertexWeight[1] +
			   boneMatrix[vertexWeightBoneID[2]] * vertexWeight[2] +
			   boneMatrix[vertexWeightBoneID[3]] * vertexWeight[3];
			   
    // Output position of the vertex, in clip space : MVP * position
    gl_Position = MVP * skin * vec4(vertexPosition_modelspace,1);

    // UV of the vertex. No special space for this one.
    UV = vertexUV;
}
