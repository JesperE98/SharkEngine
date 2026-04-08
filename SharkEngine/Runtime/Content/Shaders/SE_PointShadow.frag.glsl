#version 430 core

in vec4 vFragPos;

uniform vec3 uLightPos;
uniform float uFarPlane;

void main() 
{
	float dist = length(vFragPos.xyz - uLightPos);
	gl_FragDepth = dist / uFarPlane; // store linear depth 0-1
}