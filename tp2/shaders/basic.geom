#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 MVP;

in vec3 v_pos[];
in vec2 fragTexCoord3[];
in vec3 normal1[];
// in vec3 normal[];

out vec2 fragTexCoord;


void main(void)
{
for (int i = 0; i < 3; i++)
	{
	  gl_Position = MVP * vec4(v_pos[i], 1.0);
	  fragTexCoord = fragTexCoord3[i];
	  EmitVertex();
	}
	EndPrimitive();
}


