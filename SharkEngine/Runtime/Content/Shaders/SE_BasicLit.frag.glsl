#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D u_DiffuseMap;		// Map 1
uniform sampler2D u_SpecularMap;	// Map 2
uniform float u_Shininess;			// Shininess factor for specular highlights
uniform vec3 u_LightDir;				// Directional light
uniform vec3 u_LightColor;
uniform vec3 u_ViewPos;				// For Specular highlights
uniform vec3 u_BaseColor;
uniform bool u_UseTexture;

void main(){
	
	vec3 base;
	if(u_UseTexture){
		base = texture(u_DiffuseMap, TexCoord).rgb;
	}
	else {
		base = u_BaseColor;
	}

	// Basic Colors
	vec3 texSpecular = texture(u_SpecularMap, TexCoord).rgb;

	// Ambient
	vec3 ambient = 0.2 * base;

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDirection = normalize(-u_LightDir); // Light direction is opposite of the light's direction vector
	float diff = max(dot(norm, lightDirection), 0.0); // Diffuse component is modulated by the diffuse texture
	vec3 diffuse = diff * u_LightColor * base; // Specular

	// Specular
	vec3 viewDir = normalize(u_ViewPos - FragPos); // Reflect the light direction around the normal
	vec3 reflectDir = reflect(-lightDirection, norm); // Calculate the specular component using the specular texture
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess); // Shininess factor
	vec3 specular = u_LightColor * (spec * texSpecular); // Final color

	FragColor = vec4(ambient + diffuse + specular, 1.0); // Combine all components and set the output color
}