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

	LightType getType() const { return m_type; }

	vec3 getPosition() const { return m_position; }
	void setPosition(const vec3 &position) { m_position = position; }
	void setPosition(float x, float y, float z);

	vec3 getDirection() const { return m_direction; }
	void setDirection(const vec3 &direction) { m_direction = direction; }
	void setDirection(float x, float y, float z);

	float getInnerCutOff() const { return m_innerCutOff; }
	void setInnerCutOff(float degrees) { m_innerCutOff = degrees; }

	float getOuterCutOff() const { return m_outerCutOff; }
	void setOuterCutOff(float degrees) { m_outerCutOff = degrees; }

	Attenuation getAttenuation() const { return m_att; }
	void setAttenuation(const Attenuation &att) { m_att= att; }
	void setAttenuation(float constant, float linear, float quadratic);

	LightColor getColor() const { return m_color; }
	void setColor(const LightColor &color) { m_color = color; }
	void setColor(const vec3 &ambient, const vec3 &diffuse, const vec3 &specular) ;

	// temporary function
	void apply(std::shared_ptr<ShaderProgram> shader);
private:
	LightType m_type;
	// commont properties
	vec3 m_position;
	vec3 m_direction;
	LightColor m_color;

	// point light properties
	Attenuation m_att;

	// spot light properties
	float m_innerCutOff;
	float m_outerCutOff;
};

}
#endif 
