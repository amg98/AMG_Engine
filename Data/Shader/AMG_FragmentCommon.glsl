const int AMG_LIGHTS = 4;
const int AMG_TEXTURES = 6;
const float AMG_CELSHADING_LEVELS = 5;

in vec2 AMG_OutUV;
in vec3 AMG_OutUV3D;
in vec3 AMG_OutToLight[AMG_LIGHTS];
in float AMG_OutLightDistance[AMG_LIGHTS];
in vec3 AMG_OutNormal;
in vec3 AMG_OutToCamera;
in float AMG_OutFogValue;
in vec3 AMG_ReflectedVector;
in vec3 AMG_RefractedVector;
in float AMG_TexBlend;
in vec2 AMG_OutUV2[2];

// Lighting uniforms
struct AMG_LightF {
	vec3 color;
	vec3 attenuation;
};
uniform AMG_LightF AMG_Lights[AMG_LIGHTS];
uniform vec4 AMG_MaterialDiffuse;
uniform vec4 AMG_MaterialSpecular;
uniform float AMG_MaterialAmbient;
uniform float AMG_DiffusePower;
uniform float AMG_SpecularPower;
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