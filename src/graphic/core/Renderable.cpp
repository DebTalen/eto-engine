#include <graphcis/core/Renderable.hpp>

using namespace eto;

Renderable::Renderable(std::shared_ptr<Model> model)
	: m_model(model), m_transform(1.0f)
{
	ETO_ASSERT(m_model != nullptr);
}

void Renderable::translate(const glm::vec3 &v)
{
	m_transform = glm::translate(m_transform, v);
}

void Renderable::rotate(float degrees, const glm::vec3 &v)
{
	m_transform = glm::rotate(m_transform, glm::radians(degrees), v);
}

void Renderable::scale(const glm::vec3 &v)
{
	m_transform = glm::scale(m_transform, v);
}
