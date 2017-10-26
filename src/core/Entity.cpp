#include <core/Entity.hpp>

using namespace eto;

Entity::Entity() 
	: m_active(true)
{

}

void Entity::update() 
{
	for (const auto& it : m_comps)
		it.second->onUpdate();
}
