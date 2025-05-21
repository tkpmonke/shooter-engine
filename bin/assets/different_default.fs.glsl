#version 330 core
out vec4 FragColor;
in vec3 pos;
in vec3 norm;
void main()
{
   FragColor = vec4(norm,1)*vec4(0.5f, 0.8f, 0.4f, 1.0f);
}
