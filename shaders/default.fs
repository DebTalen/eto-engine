#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 color;


uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

uniform float shininess = 32;

uniform vec3 light_ambient = vec3(0.5);
uniform vec3 light_diffuse = vec3(1.0);
uniform vec3 light_specular = vec3(0.5);
uniform vec3 light_position;

uniform vec3 viewPos;   

void main()
{//
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light_position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	vec3 ambient = light_ambient * vec3(texture(texture_diffuse1, TexCoords))
		* vec3(texture(texture_diffuse2, TexCoords))
		* vec3(texture(texture_diffuse3, TexCoords));
	vec3 diffuse = light_diffuse * diff * vec3(texture(texture_diffuse1, TexCoords))
		* vec3(texture(texture_diffuse2, TexCoords))
		* vec3(texture(texture_diffuse3, TexCoords));
	vec3 specular = light_specular * spec * vec3(texture(texture_specular1, TexCoords))
		* vec3(texture(texture_specular2, TexCoords));
	//	* vec3(texture(texture_specular3, TexCoords));

	color = vec4(ambient + diffuse + specular, 1.0);
	//color = vec4(1, 1, 1, 1);
}
