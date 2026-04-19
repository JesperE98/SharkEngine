#version 330 core

out vec4 FragColor;

uniform vec3 uLightDir;		// Directional Light Direction (normalized)
uniform vec3 uViewDir;		// Direction from camera to current fragment
uniform vec3 uSunColor;		// e.g. vec3(1.0, 0.95, 0.8) for warm white
uniform float uSunSize;

void main() 
{
	vec3 viewDir = normalize(uViewDir);
	vec3 sunDir = normalize(-uLightDir); // Light points towards scene, sun is opposite

	float sun = dot(viewDir, sunDir);

	float disc = step(uSunSize, sun);

	// Optional soft glow halo around it
	float glow = pow(max(sun, 0.0), 256.0) * 0.5;

	vec3 color = uSunColor * disc + uSunColor * glow;

	FragColor = vec4(color, 1.0);
}