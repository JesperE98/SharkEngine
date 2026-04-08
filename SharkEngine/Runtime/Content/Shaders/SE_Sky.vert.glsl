#version 330 core

layout(location = 0) in vec2 aPos;

uniform mat4 uInvProjection;
uniform mat4 uInvView;

out vec3 vViewDir;

void main() 
{
	// Fullscreen triangle trick
	vec4 clipPos = vec4(aPos.xy, 1.0, 1.0);

	// Reconstruct world-space view direction
	vec4 viewPos = uInvProjection * clipPos;
	viewPos.w = 0.0;
	vViewDir = (uInvView * viewPos).xyz;

	gl_Position = clipPos;
}