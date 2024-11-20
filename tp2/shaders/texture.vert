#version 330 core
layout(location = 0) in vec3 inPosition;  // Position du sommet
layout (location = 1) in vec3 normal;
layout(location = 2) in vec2 inTexCoord; // Coordonnées de texture


out vec2 fragTexCoord;

uniform mat4 MVP; // Matrice de Modèle-Vue-Proj

void main()
{
    gl_Position = MVP * vec4(inPosition, 1.0); // Transformation en clip space
    fragTexCoord = inTexCoord;   
}