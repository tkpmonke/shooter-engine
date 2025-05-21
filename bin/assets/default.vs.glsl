#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
out vec3 pos;
out vec3 norm;
void main()
{
   pos = (aPos+1)/2;
	norm = (aNormal+1)/2;
	gl_Position = u_projection*u_view*u_model*vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
