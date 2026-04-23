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
uniform sampler2D uDiffuseMap;		// Map 0
uniform sampler2D uSpecularMap;		// Map 1

// --- MULTIPLE DIRECTIONAL SHADOWS ---
#define MAX_DIR_SHADOWS 4
uniform sampler2DShadow uShadowMaps[MAX_DIR_SHADOWS]; // Units 2, 3, 4, 5
uniform mat4 uLightSpaceMatrices[MAX_DIR_SHADOWS];

// --- MULTIPLE POINT SHADOWS ---
#define MAX_POINT_SHADOWS 2
uniform samplerCube uPointShadowMaps[MAX_POINT_SHADOWS]; // matches MAX_SHADOW_POINT_LIGHTS
uniform float uPointShadowFarPlane;

// -- Material Properties --
uniform float	uShininess;			// Shininess factor for specular highlights
uniform vec3	uBaseColor;
uniform bool	uUseTexture;
uniform bool	uUseSpecularMap;
uniform vec2	uTextureTiling;

// -- Light & Camera Properties --
#define MAX_LIGHTS 8
uniform LightData uLights[MAX_LIGHTS];
uniform int uLightCount;
uniform vec3 uLightDir;				// Directional light
uniform vec3 uLightColor;
uniform vec3 uViewPos;				// For Specular highlights


float CalculateDirShadow(int mapIndex, vec3 normal, vec3 lightDir) {
	vec4 fragPosLightSpace = uLightSpaceMatrices[mapIndex] * vec4(vFragPos, 1.0);
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w; // Perspective divide 
	projCoords = projCoords * 0.5 + 0.5; // Transform to [0, 1] range for texture sampling

	// Keep shadows 0.0 if outside the light's far plane
	if(projCoords.x < 0.0 || projCoords.x > 1.0 ||
	   projCoords.y < 0.0 || projCoords.y > 1.0 ||
	   projCoords.z > 1.0) return 0.0; 

	// Shadow Bias (prevents "Shadow Acne" / Moire patterns)
	// We can vary the bias based on the angle to the light
	float bias = max(0.002 * (1.0 - dot(normal, lightDir)), 0.0001);

	// Check if pixel is in shadow
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(uShadowMaps[mapIndex], 0); // Gets size of one pixel

	for(int x = -2; x <= 2; ++x) {
		for(int y = -2; y <= 2; ++y) {
			shadow += texture(uShadowMaps[mapIndex], vec3(projCoords.xy + vec2(x, y) * texelSize, projCoords.z - bias));
		}
	}

	return (1.0 - (shadow / 25.0));
}

float CalculatePointShadow(int lightIndex, vec3 fragPos, vec3 lightPos) {
	vec3 fragToLight = fragPos - lightPos;
	float currentDepth = length(fragToLight) / uPointShadowFarPlane;

	if(currentDepth > 1.0) return 0.0;

	float bias = 0.05;
	return currentDepth - bias > texture(uPointShadowMaps[lightIndex], fragToLight).r ? 1.0 : 0.0;
}

void main(){
	// Get Base Color wether we use a texture or not, same for the specular
	vec2 uv = vTexCoord * uTextureTiling;

	vec3 baseColor	= uUseTexture ? texture(uDiffuseMap, uv).rgb : uBaseColor;

	vec3 specFactor;
		if (uUseSpecularMap) {
			vec3 roughnessSample = texture(uSpecularMap, uv).rgb;
			specFactor = vec3(1.0) - roughnessSample; 
		} else {
			specFactor = vec3(0.05); // Default low shine for non-mapped objects
		}

	vec3 norm = normalize(vNormal);
	vec3 viewDir = normalize(uViewPos - vFragPos);

	vec3 totalLighting = vec3(0.0);
	
	int dirShadowIndex = 0;
	int pointShadowIndex = 0;

	// Loop through all active lights
	for(int i = 0; i < uLightCount; i++) {
		vec3 lightDir;
		float attenuation = 1.0;
		float shadowFactor = 0.0;


		if(uLights[i].type == 0) { // Directional Light
			lightDir = normalize(-uLights[i].direction);
			if(dirShadowIndex < MAX_DIR_SHADOWS){
				shadowFactor = CalculateDirShadow(dirShadowIndex, norm, lightDir);
				dirShadowIndex++;
			}
		}
		else if(uLights[i].type == 1) { // point light
			vec3 lightVec	= uLights[i].position - vFragPos;
            float distance	= length(lightVec);
            lightDir		= normalize(lightVec);
            attenuation		= clamp(1.0 - distance / uLights[i].range, 0.0, 1.0);
            
            if(pointShadowIndex < MAX_POINT_SHADOWS) {
                shadowFactor = CalculatePointShadow(pointShadowIndex, vFragPos, uLights[i].position);
                pointShadowIndex++;
            }
		}

		// Diffuse
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * uLights[i].color * uLights[i].intensity * baseColor;

		// Specular (using Blinn-Phong method)
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float specAmount = pow(max(dot(norm, halfwayDir), 0.0), uShininess);
		vec3 specular = specAmount * uLights[i].color * uLights[i].intensity * specFactor;

		totalLighting += (1.0 - shadowFactor) * (diffuse + specular) * attenuation;
	}

	// Adding a small constant ambient so the scene isn't pitch black
	vec3 ambient = 0.1 * baseColor;
	vec3 finalColor = ambient + totalLighting;

	FragColor = vec4(finalColor, 1.0);
}