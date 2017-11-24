#ifndef ETO_RENDERABLE 
#define ETO_RENDERABLE

#include <resources/Model.hpp>
#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;
using glm::mat4;

namespace eto
{

class Renderable
{
public:
	std::shared_ptr<Model> model;
	mat4 transform = mat4(1.0f); // model to world matrix
};

}

#endif 
