#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;

out vec3 pos;

void main()
{
	pos = position - vec3(0.0f, 0.02f, 0.0f);
}