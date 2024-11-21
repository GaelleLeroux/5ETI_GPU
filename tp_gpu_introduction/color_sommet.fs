#version 330 core

in vec2 TexCoord;
in vec3 Color;

out vec4 FragColor;

uniform sampler2D myTextureSampler;

void main() {
    vec4 textureColor = texture(myTextureSampler, TexCoord);
    FragColor = vec4(Color, 1.0) * textureColor; // Mélange couleur et texture
}
