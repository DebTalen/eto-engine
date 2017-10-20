#include <graphcis/core/Light.hpp>

using namespace eto;

Light::Light (LightType type, const vec3 &position, const vec3 &direction)
	: m_type(type), m_position(position), m_direction(direction)
{
	m_innerCutOff = 13;
	m_outerCutOff = 18;
}

void Light::setPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void Light::setDirection(float x, float y, float z)
{
	m_direction.x = x;
	m_direction.y = y;
	m_direction.z = z;
}

void Light::setAttenuation(float constant, float linear, float quadratic)
{
	m_att.constant = constant;
	m_att.linear = linear;
	m_att.quadratic = quadratic;
}

void Light::setColor(const vec3 &ambient, const vec3 &diffuse, const vec3 &specular) 
{
	m_color.ambient = ambient;
	m_color.diffuse = diffuse;
	m_color.specular = specular;
}

void Light::apply(SPtr<ShaderProgram> shader)
{
	shader->use();
	shader->setVec3f("light.ambient", m_color.ambient);
	shader->setVec3f("light.diffuse", m_color.diffuse);
	shader->setVec3f("light.specular", m_color.specular);
	shader->setVec3f("light.position", m_position);
	shader->setVec3f("light.direction", m_direction);
	shader->setInt("light.type", static_cast<int>(m_type));
	if (m_type == Point || m_type == Spot)
	{
		shader->setFloat("light.constant", m_att.constant);
		shader->setFloat("light.linear", m_att.linear);
		shader->setFloat("light.quadratic", m_att.quadratic);
		if (m_type == Spot) 
		{
			shader->setFloat("light.innerCurOff", m_innerCutOff);
			shader->setFloat("light.outerCurOff", m_outerCutOff);
		}
	}
}
