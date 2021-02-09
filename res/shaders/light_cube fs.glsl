#version 330 core

uniform vec3 FragColor;
out vec4 final_color;

void main()
{
    final_color = vec4(FragColor, 1.0); // set alle 4 vector values to 1.0
}