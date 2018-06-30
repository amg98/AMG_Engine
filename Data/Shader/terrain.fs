#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 normal;
in vec3 toLight;
in vec3 toCamera;
in float fog_value;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform vec3 lightColor;

uniform float fog_density;
uniform vec4 fog_color;
uniform float fog_gradient;

void main(){

	float brightness = max(dot(normal, toLight), 0.2);
	vec4 diffuse = vec4(brightness * lightColor, 1);

    // Output color = color of the texture at the specified UV
    color = diffuse * texture(myTextureSampler, UV);
    
    color = mix(fog_color, color, fog_value);
}
