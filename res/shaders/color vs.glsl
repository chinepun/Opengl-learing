// #version 330 core
// layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aNormal;
// layout (location = 2) in vec2 aTexCoords;

// uniform mat4 model;
// uniform mat4 view;
// uniform mat4 projection;

// out vec3 FragPos;
// out vec3 Normal;
// out vec2 TexCoords;

// void main()
// {
// 	gl_Position = projection * view * model * vec4(aPos, 1.0);
// // transforming normal vectors to world space co-ordinates,
// // should be mat3 so it loses translation properties, if doing non-uniform scaling,
// // it is essential to mulitiply normal vector with normal matrix
// 	Normal = mat3(transpose(inverse(model))) * aNormal;
// 	FragPos = vec3(model * vec4(aPos, 1.0f));// fragment position in world space
// 	TexCoords = aTexCoords;
//}


#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

