#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform vec3 viewPos;
uniform float shininess = 32;
uniform vec3 light_ambient = vec3(1);
uniform vec3 light_diffuse = vec3(1.0);
uniform vec3 light_specular = vec3(0.5);
uniform vec3 light_position;
uniform vec3 color_ambient;
uniform vec3 color_diffuse;
uniform vec3 color_specular;

void main()
{//
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light_position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	vec3 ambient = color_ambient * light_ambient * diff;
	vec3 diffuse = color_diffuse * light_diffuse * diff; 
	vec3 specular = color_specular * light_specular * spec;
	color = vec4(ambient + diffuse + specular, 1.0);
//	color = vec4(color_ambient + color_diffuse + color_specular, 1);
}
