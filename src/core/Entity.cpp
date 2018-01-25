#include <core/Entity.hpp>
#include <core/EntityManager.hpp>

using namespace eto::core;

Entity::Entity(EntityManager &emanager, ComponentManager &cmanager, eid id)
        : m_id(id), m_valid(1),
          m_emanager(emanager), 
	  m_cmanager(cmanager)
{

}

Entity::Entity(const Entity &&rhs)
        : m_id(rhs.m_id), m_valid(1), 
  	  m_emanager(rhs.m_emanager), 
	  m_cmanager(rhs.m_cmanager)
{
        // rhs.m_valid = false; should be allrigth
}

void Entity::destroy()
{
        ETO_ASSERT (m_valid);
        m_emanager.destroy(*this);
}


bool Entity::operator==(const Entity &rhs) const
{
        return (&m_emanager == &rhs.m_emanager &&
                m_valid == rhs.m_valid 	       &&
                m_id    == rhs.m_id );
}

bool Entity::operator!=(const Entity &rhs) const
{
        return !(*this == rhs);
}

