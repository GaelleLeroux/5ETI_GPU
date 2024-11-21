#version 330 core
out vec4 FragColor;

in vec2 TexCoord; // Coordonnées de texture normalisées

uniform sampler2D myTextureSampler; // Texture d'entrée
uniform float circleRadius;         // Rayon du cercle (en fraction de l'image)

void main()
{
    // Centre de l'image en coordonnées de texture
    vec2 center = vec2(0.5, 0.5);

    // Calcul de la distance entre le fragment et le centre
    float distance = length(TexCoord - center);

    // Si le fragment est dans le rayon, il devient rouge, sinon on affiche la texture
    if (distance <= circleRadius) {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Rouge
    } else {
        FragColor = texture(myTextureSampler, TexCoord); // Texture originale
    }
}
