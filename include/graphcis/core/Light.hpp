#ifndef ETO_LIGHT
#define ETO_LIGHT

#include <glm/glm.hpp>
// temporary
#include <graphcis/core/ShaderProgram.hpp>
#include <memory>

using glm::vec3;

namespace eto
{

class Light
{
public:
	enum LightType { Directional = 0, Point, Spot };
	struct Attenuation {
		float constant = 1.0f;
		float linear = 0.045;
		float quadratic = 0.0075;
	};
	struct LightColor {
		vec3 ambient = vec3(0.5);
		vec3 diffuse = vec3(1.0);
		vec3 specular = vec3(0.5);
	};

	Light (LightType type, const vec3 &position, const vec3 &direction);

	LightType get_type() const { return m_type; }

	vec3 get_position() const { return m_position; }
	void set_position(const vec3 &position) { m_position = position; }
	void set_position(float x, float y, float z);

	vec3 get_direction() const { return m_direction; }
	void set_direction(const vec3 &direction) { m_direction = direction; }
	void set_direction(float x, float y, float z);

	float get_inner_cut_off() const { return m_inner_cut_off; }
	void set_inner_cut_off(float degrees) { m_inner_cut_off = degrees; }

	float get_outer_cut_off() const { return m_outer_cut_off; }
	void set_outer_cut_off(float degrees) { m_outer_cut_off = degrees; }

	Attenuation get_attenuation() const { return m_att; }
	void set_attenuation(const Attenuation &att) { m_att= att; }
	void set_attenuation(float constant, float linear, float quadratic);

	LightColor get_color() const { return m_color; }
	void set_color(const LightColor &color) { m_color = color; }
	void set_color(const vec3 &ambient, const vec3 &diffuse, const vec3 &specular) ;

private:
	friend class Renderer;
	LightType m_type;
	// commont properties
	vec3 m_position;
	vec3 m_direction;
	LightColor m_color;

	// point light properties
	Attenuation m_att;

	// spot light properties
	float m_inner_cut_off;
	float m_outer_cut_off;
};

}
#endif 
