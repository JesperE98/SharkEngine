#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D u_Texture;
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 ambientColor;

void main(){
	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, -lightDir), 0.0);

	vec3 texColor = texture(u_Texture, TexCoord).rgb;

	vec3 ambient = ambientColor * texColor;
	vec3 diffuse = diff * lightColor * texColor;

	vec3 result = ambient + diffuse;

	// FragColor = vec4(result, 1.0);
	FragColor = texture(u_Texture, TexCoord);
}