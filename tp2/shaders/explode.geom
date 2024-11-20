#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 MVP;
uniform float time;

in vec3 v_pos[];
in vec2 fragTexCoord3[];
in vec3 normal1[];
// in vec3 normal[];

out vec2 fragTexCoord;


void main(void)
{
vec3 normal = cross(v_pos[1]-v_pos[0],v_pos[2]-v_pos[0]);

for (int i = 0; i < 3; i++)
	{
    
      vec3 new_pos = v_pos[i] + (normal) * abs(sin(time)*10);

	  gl_Position = MVP * vec4(new_pos, 1.0);
	  fragTexCoord = fragTexCoord3[i];
	  EmitVertex();
	}
	EndPrimitive();
}


