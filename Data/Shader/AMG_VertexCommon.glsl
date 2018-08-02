const int AMG_MAX_BONES = 16;
const int AMG_LIGHTS = 4;

struct AMG_LightV {
	vec3 position;
};

uniform mat4 AMG_MVP;
uniform mat4 AMG_BoneMatrix[AMG_MAX_BONES];
uniform AMG_LightV AMG_Light[AMG_LIGHTS];
uniform mat4 AMG_MV;
uniform mat4 AMG_M;
uniform float AMG_FogDensity;
uniform float AMG_FogGradient;
uniform vec3 AMG_CamPosition;
uniform vec2 AMG_TexScale;
uniform vec4 AMG_TexPosition;

out vec2 AMG_OutUV;
out vec3 AMG_OutToLight[AMG_LIGHTS];
out float AMG_OutLightDistance[AMG_LIGHTS];
out vec3 AMG_OutNormal;
out vec3 AMG_OutToCamera;
out float AMG_OutFogValue;
out vec3 AMG_ReflectedVector;
out vec3 AMG_RefractedVector;
out vec2 AMG_OutUV2[2];
out float AMG_TexBlend;
out vec3 AMG_OutUV3D;
