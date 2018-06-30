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
out vec3 normal;
out vec3 toLight;
out vec3 toCamera;
out float fog_value;

// Uniforms
uniform vec3 lightPosition;
uniform mat4 boneMatrix[MAX_BONES];
uniform mat4 MVP;
uniform mat4 MV;

uniform float fog_density;
uniform float fog_gradient;

void main(){

	mat4 skin = boneMatrix[vertexWeightBoneID[0]] * vertexWeight[0] +
			   boneMatrix[vertexWeightBoneID[1]] * vertexWeight[1] +
			   boneMatrix[vertexWeightBoneID[2]] * vertexWeight[2] +
			   boneMatrix[vertexWeightBoneID[3]] * vertexWeight[3];
    
    // Output position of the vertex, in clip space : MVP * position
    gl_Position = MVP * skin * vec4(vertexPosition_modelspace,1);
    
    UV = vertexUV;
    mat4 mvskin = MV * skin;
    normal = normalize((mvskin * vec4(vertexNormal, 0)).xyz);
    toLight = normalize(lightPosition - (mvskin * vec4(vertexPosition_modelspace, 1)).xyz);
    toCamera = -(mvskin * vec4(vertexPosition_modelspace, 1)).xyz;
    float distance = length(toCamera);
    toCamera /= distance;
    
    fog_value = clamp(exp(-pow(fog_density*distance, fog_gradient)), 0, 1);
}
