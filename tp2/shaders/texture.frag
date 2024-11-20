#version 330 core

in vec2 fragTexCoord; // Coordonnées de texture provenant du vertex shader
out vec4 FragColor;   // Couleur finale du fragment

uniform sampler2D textureSampler; // Texture 2D à appliquer

void main()
{
    // Récupère la couleur de la texture à partir des coordonnées
    FragColor = texture(textureSampler, fragTexCoord);
}
