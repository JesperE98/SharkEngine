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
in vec4 vFragPosLightSpace;

out vec4 FragColor;

// -- Texture Samplers --
uniform sampler2D uDiffuseMap;		// Map 1
uniform sampler2D uSpecularMap;	// Map 2
uniform sampler2D uShadowMap; // Map 3

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


float CalculateShadow(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
	
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w; // Perspective divide 
	projCoords = projCoords * 0.5 + 0.5; // Transform to [0, 1] range for texture sampling

	if(projCoords.z > 1.0) return 0.0; // Keep shadows 0.0 if outside the light's far plane

	float currentDepth = projCoords.z; // Get the current depth of pixel from light's perspective

	// Shadow Bias (prevents "Shadow Acne" / Moire patterns)
	// We can vary the bias based on the angle to the light
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

	// Check if pixel is in shadow
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(uShadowMap, 0); // Gets size of one pixel

	for(int x = -1; x <= 1; ++x) {
		for(int y = -1; y <= 1; ++y) {
			float pchDepth = texture(uShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pchDepth ? 1.0 : 0.0;
		}
	}

	return shadow / 9.0;
}

void main(){
	// Get Base Color wether we use a texture or not, same for the specular
	vec3 baseColor	= uUseTexture ? texture(uDiffuseMap, vTexCoord).rgb : uBaseColor;
	vec3 specFactor = uUseSpecularMap ? texture(uSpecularMap, vTexCoord).rgb : vec3(1.0);
//	vec3 specFactor = texture(uSpecularMap, vTexCoord).rgb;

	vec3 norm = normalize(vNormal);
	vec3 viewDir = normalize(uViewPos - vFragPos);

	vec3 totalLighting = vec3(0.0);

	// Loop through all active lights
	for(int i = 0; i < uLightCount; i++) {
		vec3 lightDir;
		float attenuation = 1.0;
		float shadowFactor = 0.0;

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

		// --- SHADOW CALCULATION ---
		if(i == 0){
			shadowFactor = CalculateShadow(vFragPosLightSpace, norm, lightDir);
		}

		totalLighting += (1.0 - shadowFactor) * (diffuse + specular) * attenuation;
	}

	// Adding a small constant ambient so the scene isn't pitch black
	vec3 ambient = 0.1 * baseColor;
	vec3 finalColor = ambient + (totalLighting * baseColor);
//	FragColor = vec4(vec3(CalculateShadow(vFragPosLightSpace, norm, uLightDir)), 1.0);
	FragColor = vec4(finalColor, 1.0);
}