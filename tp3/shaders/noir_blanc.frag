#version 330 core

in vec2 uv;

out vec4 color;

uniform sampler2D myTextureSampler;

void main()
{
    vec3 color_c = texture( myTextureSampler, uv ).rgb;
    
    float gray = 0.299 * color_c.r + 0.587 * color_c.g + 0.114 * color_c.b;

    color = vec4(vec3(gray), 1.0);
}
