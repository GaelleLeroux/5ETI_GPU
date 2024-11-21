#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D myTextureSampler;

void main()
{
    // Récupère la couleur du texel
    vec3 color = texture(myTextureSampler, TexCoord).rgb;

    // Calcule la luminosité en utilisant une pondération standard
    float gray = 0.299 * color.r + 0.587 * color.g + 0.114 * color.b;

    // Crée une couleur en niveaux de gris
    FragColor = vec4(vec3(gray), 1.0);
}