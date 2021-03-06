const int AMG_LIGHTS = 4;
const int AMG_TEXTURES = 6;
const float AMG_CELSHADING_LEVELS = 5;
const int AMG_PCF_SIZE = 4;
const int AMG_PCF_TEXELS = (AMG_PCF_SIZE * 2 + 1) * (AMG_PCF_SIZE * 2 + 1);
const int AMG_DLIGHTS = 8;

in vec2 AMG_OutUV;
in vec3 AMG_OutUV3D;
in vec3 AMG_OutToLight[AMG_LIGHTS];
in float AMG_OutLightDistance[AMG_LIGHTS];
in vec3 AMG_OutNormal;
in vec3 AMG_OutToCamera;
in float AMG_OutFogValue;
in float AMG_TexBlend;
in vec2 AMG_OutUV2[2];
in vec4 AMG_OutShadowCoord;
in vec3 AMG_RView;
in vec3 AMG_RNormal;
in vec3 AMG_OutGPosition;
in vec3 AMG_OutGNormal;
in mat3 AMG_OutGNormalMatrix;

// Lighting uniforms
struct AMG_LightF {
	vec3 color;
	vec3 attenuation;
	vec3 spotDirection;
	float spotCutoff;
};
struct AMG_LightD {
	vec3 position;
	vec3 color;
	vec3 attenuation;
	vec3 spotDirection;
	float spotCutoff;
};
uniform AMG_LightF AMG_Lights[AMG_LIGHTS];
uniform AMG_LightD AMG_LightDR[AMG_DLIGHTS];
uniform vec4 AMG_MaterialDiffuse;
uniform vec4 AMG_MaterialSpecular;
uniform float AMG_MaterialAmbient;
uniform float AMG_DiffusePower;
uniform float AMG_SpecularPower;
uniform float AMG_SpecularReflectivity;
uniform vec4 AMG_FogColor;
uniform sampler2D AMG_TextureSampler[AMG_TEXTURES];
uniform samplerCube AMG_TextureCubeSampler;
uniform vec4 AMG_SprColor;
uniform float AMG_TexProgress;
uniform float AMG_CharWidth;
uniform float AMG_CharEdge;
uniform float AMG_CharBorderWidth;
uniform float AMG_CharBorderEdge;
uniform vec2 AMG_CharShadowOffset;
uniform vec3 AMG_CharOutlineColor;
uniform float AMG_ShadowMapSize;
uniform float AMG_WorldAmbient;
uniform float AMG_RefractionIndex;
