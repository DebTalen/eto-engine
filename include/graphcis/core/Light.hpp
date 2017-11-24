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

	Light (LightType type = Directional, const vec3 &position = vec3(0), const vec3 &direction = vec3(0))
		: type(type), position(position), direction(direction)
	{
		inner_cut_off = 13;
		outer_cut_off = 18;
	}


	//  Member data
	LightType type;
	// commont properties
	vec3 position;
	vec3 direction;
	LightColor color;

	// point light properties
	Attenuation att;

	// spot light properties
	float inner_cut_off;
	float outer_cut_off;
};

}
#endif 
