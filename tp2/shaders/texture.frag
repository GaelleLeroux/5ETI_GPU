#version 330 core

in vec2 fragTexCoord; // Coordonnées de texture provenant du vertex shader
out vec4 FragColor;   // Couleur finale du fragment

uniform sampler2D textureSampler; // Texture 2D à appliquer
uniform float time; // Temps (en secondes)

void main()
{
    // Récupère la couleur de la texture à partir des coordonnées
    // Applique un décalage périodique basé sur le temps
    
    FragColor = texture(textureSampler, fragTexCoord);
}
