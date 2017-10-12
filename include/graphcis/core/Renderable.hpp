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
	Renderable (SPtr<Model> model);

	void translate(const glm::vec3 &v);

	void rotate(float degrees, const glm::vec3 &v);

	void scale(const glm::vec3 &v);

	mat4 getTransform() const { return m_transform; }
	
	//temorary functions
	void draw();
private:
	SPtr<Model> m_model;
	mat4 	    m_transform; // model to world matrix
};

}

#endif 
