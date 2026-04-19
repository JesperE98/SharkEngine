#version 330 core

layout (location = 0) in vec3 aPos; // Input vertex position from the vertex buffer (VBO)
layout (location = 1) in vec3 aColor; // Optional: Input vertex color (if using per-vertex color)
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

// Output to fragment shader
out vec3 vertexColor;

uniform mat4 model;			// Objects local transform
uniform mat4 view;			// Camera view matrix
uniform mat4 projection;	// Perspective projection matrix

void main(){
	// Pass color to fragment shader
	vertexColor = aColor;

	// Transform vertex position from local space to clip space
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}