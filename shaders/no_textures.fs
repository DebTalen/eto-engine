#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 color;

struct Light {
	int type;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
	vec3 direction;

	float constant;
	float linear;
	float quadratic;

	float outerCutOff;
	float innerCutOff;
};
uniform Light light;

uniform vec3 view_position;
uniform vec3 color_ambient;
uniform vec3 color_diffuse;
uniform vec3 color_specular;
uniform float shininess = 32;

void main()
{//
	vec3 norm = normalize(Normal);
	vec3 ambient, diffuse, specular;
	if (light.type == 0)
	{
		vec3 lightDir = normalize(-light.direction);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 viewDir = normalize(view_position - FragPos);
		vec3 reflectDir = reflect(lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

		ambient = color_ambient * light.ambient ;
		diffuse = light.diffuse * (color_diffuse * diff); 
		specular = light.specular * (color_specular * spec);
	}
	else if (light.type == 1 || light.type == 2)
	{
		vec3 lightDir = normalize(light.position - FragPos);
		// diffuse shading
		float diff = max(dot(norm,lightDir), 0.0);
		// specular shading
		vec3 viewDir = normalize(view_position - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
		// attenuation
		float distance    = length(light.position - FragPos);
		float attenuation = 1.0 / (light.constant + light.linear * distance + 
					   light.quadratic * (distance * distance));    
		ambient = color_ambient * light.ambient ;
		diffuse = light.diffuse * (color_diffuse * diff); 
		specular = light.specular * (color_specular * spec);
		ambient  *= attenuation;
		diffuse  *= attenuation;
		specular *= attenuation;
		if (light.type == 2) 
		{
			float theta     = dot(lightDir, normalize(-light.direction));
			float epsilon   = light.innerCutOff - light.outerCutOff;
			float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); 
			diffuse  *= intensity;
			specular *= intensity;
		}
	}
	color = vec4(ambient + diffuse + specular, 1.0);
	//	color = vec4(color_ambient + color_diffuse + color_specular, 1);
}
