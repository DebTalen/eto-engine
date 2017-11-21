#include <graphcis/core/Light.hpp>

using namespace eto;

Light::Light (LightType type, const vec3 &position, const vec3 &direction)
	: m_type(type), m_position(position), m_direction(direction)
{
	m_inner_cut_off = 13;
	m_outer_cut_off = 18;
}

void Light::set_position(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void Light::set_direction(float x, float y, float z)
{
	m_direction.x = x;
	m_direction.y = y;
	m_direction.z = z;
}

void Light::set_attenuation(float constant, float linear, float quadratic)
{
	m_att.constant = constant;
	m_att.linear = linear;
	m_att.quadratic = quadratic;
}

void Light::set_color(const vec3 &ambient, const vec3 &diffuse, const vec3 &specular) 
{
	m_color.ambient = ambient;
	m_color.diffuse = diffuse;
	m_color.specular = specular;
}
