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
uniform LightData uLightdata[MAX_LIGHTS];
uniform int uLightCount;
uniform vec3 uLightDir;				// Directional light
uniform vec3 uLightColor;
uniform vec3 uViewPos;				// For Specular highlights


void main(){
	vec3 baseColor	= uUseTexture ? texture(uDiffuseMap, vTexCoord).rgb : uBaseColor;
	vec3 specFactor = uUseSpecularMap ? texture(uSpecularMap, vTexCoord).rgb : vec3(1.0);

//	vec3 base;
//	if(uUseTexture){
//		base = texture(uDiffuseMap, vTexCoord).rgb;
//	}
//	else {
//		base = uBaseColor;
//	}
//
//	// Ambient Lighting 
//	float ambientStrength = 0.15;
//	vec3 ambient = ambientStrength * uLightColor * base;
//
//	// Diffuse Lighting
//	vec3 norm = normalize(vNormal);
//	vec3 lightDirNormalized = normalize(-uLightDir); // Light direction is opposite of the light's direction vector
//	float diff = max(dot(norm, lightDirNormalized), 0.0); // Diffuse component is modulated by the diffuse texture
//	vec3 diffuse = diff * uLightColor * base; // Specular
//
//	// Specular (Blinn-Phong)
//	vec3 viewDir = normalize(uViewPos - vFragPos); // Reflect the light direction around the normal
////	vec3 reflectDir = reflect(-lightDirNormalized, norm); // Calculate the specular component using the specular texture
//	vec3 halfwayDir = normalize(lightDirNormalized + viewDir); // Using Halfway Vector for Blinn-Phong since its more stable than reflect
//	float specAmount = pow(max(dot(norm, halfwayDir), 0.0), uShininess); // Shininess factor
//	
//	// Sample Specular Map or fallback to full white (1.0)
//	vec3 specFactor;
//	if(uUseSpecularMap){
//		specFactor = texture(uSpecularMap, vTexCoord).rgb;
//	} else {
//		specFactor = vec3(1.0);
//	}
//
//	vec3 specular = uLightColor * (specAmount * specFactor);
//
//	FragColor = vec4(ambient + diffuse + specular, 1.0); // Combine all components and set the output color
}