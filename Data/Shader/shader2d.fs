#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec4 color;

// Ouput data
out vec4 outColor;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

void main(){

    // Output color = color of the texture at the specified UV
    outColor = color * texture(myTextureSampler, UV);
}
