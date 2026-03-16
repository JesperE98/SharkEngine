#version 330 core

struct LightData {
	int type;
	vec3 position;
	vec3 direction;
	vec3 color;
	float intensity;
	float range;
};

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vTexCoord;

out vec4 FragColor;

// -- Texture Samplers --
uniform sampler2D uDiffuseMap;		// Map 1
uniform sampler2D uSpecularMap;	// Map 2

// -- Material Properties --
uniform float uShininess;			// Shininess factor for specular highlights
uniform vec3 uBaseColor;
uniform bool uUseTexture;
uniform bool uUseSpecularMap;

// -- Light & Camera Properties --
#define MAX_LIGHTS 8
uniform LightData uLights[MAX_LIGHTS];
uniform int uLightCount;
uniform vec3 uLightDir;				// Directional light
uniform vec3 uLightColor;
uniform vec3 uViewPos;				// For Specular highlights


void main(){
	// Get Base Color wether we use a texture or not, same for the specular
	vec3 baseColor	= uUseTexture ? texture(uDiffuseMap, vTexCoord).rgb : uBaseColor;
	vec3 specFactor = uUseSpecularMap ? texture(uSpecularMap, vTexCoord).rgb : vec3(1.0);

	vec3 norm = normalize(vNormal);
	vec3 viewDir = normalize(uViewPos - vFragPos);

	vec3 totalLighting = vec3(0.0);

	// Loop through all active lights
	for(int i = 0; i < uLightCount; i++) {
		vec3 lightDir;
		float attenuation = 1.0;

		if(uLights[i].type == 0) { // Directional Light
			lightDir = normalize(-uLights[i].direction);
		}
		else { // Point Light
			vec3 lightVec = uLights[i].position - vFragPos;
			float distance = length(lightVec);
			lightDir = normalize(lightVec);

			// This is a simple linear attenuation based on range value
			attenuation = clamp(1.0 - distance / uLights[i].range, 0.0, 1.0);
		}

		// Diffuse
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * uLights[i].color * uLights[i].intensity;

		// Specular (using Blinn-Phong method)
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float specAmount = pow(max(dot(norm, halfwayDir), 0.0), uShininess);
		vec3 specular = specAmount * uLights[i].color * uLights[i].intensity * specFactor;

		totalLighting += (diffuse + specular) * attenuation;
	}

	// Adding a small constant ambient so the scene isn't pitch black
	vec3 ambient = 0.1 * baseColor;

	FragColor = vec4(ambient + (totalLighting * baseColor), 1.0);
}