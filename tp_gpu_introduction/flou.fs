#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D myTextureSampler;

// Offsets pour un filtre 3x3
const float offset = 5.0 / 300.0; // Ajustez selon la taille de votre texture
const vec2 offsets[9] = vec2[](
    vec2(-offset,  offset), // Haut-gauche
    vec2( 0.0,     offset), // Haut-centre
    vec2( offset,  offset), // Haut-droite
    vec2(-offset,  0.0),    // Milieu-gauche
    vec2( 0.0,     0.0),    // Centre
    vec2( offset,  0.0),    // Milieu-droite
    vec2(-offset, -offset), // Bas-gauche
    vec2( 0.0,    -offset), // Bas-centre
    vec2( offset, -offset)  // Bas-droite
);

// Poids du filtre (noyau gaussien 3x3)
const float kernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16
);

void main()
{
    vec4 color = vec4(0.0); // Couleur accumulée

    // Appliquer le filtre convolutionnel
    for (int i = 0; i < 9; i++) {
        color += texture(myTextureSampler, TexCoord + offsets[i]) * kernel[i];
    }

    FragColor = color; // Résultat final flouté
}
