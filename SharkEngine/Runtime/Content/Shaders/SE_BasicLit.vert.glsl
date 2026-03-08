#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aNormal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main(){
	vec4 worldPos = u_Model * vec4(aPos, 1.0);
	FragPos = worldPos.xyz;

	Normal = mat3(transpose(inverse(u_Model))) * aNormal;

	TexCoord = aTexCoord;

	gl_Position = u_Projection * u_View * worldPos;
}