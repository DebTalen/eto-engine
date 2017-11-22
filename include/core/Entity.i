#include <core/EntityManager.hpp>

using namespace eto;

inline Entity::Entity(EntityManager &manager, size_t id)
        : m_manager(&manager), m_id(id), m_valid(1)
{

}

inline  Entity::Entity(Entity &&rhs)
{
        m_manager = rhs.m_manager;
        m_id = rhs.m_id;
        m_valid = true;
        rhs.m_valid = false;
}

inline void Entity::destroy()
{
        ETO_ASSERT (m_valid);
        m_manager->destroy(*this);
}

        template <typename T, typename ...Args>
inline T& Entity::add(Args&& ...args)
{
        ETO_ASSERT (m_valid);
        return m_manager->create_component<T>(*this, std::forward<Args>(args)...);
}

template <typename T>
inline T& Entity::get() const 
{
        ETO_ASSERT (m_valid);
        return m_manager->get_component<T>(*this);
}

        template <typename T>
inline void Entity::remove() 
{ 
        ETO_ASSERT (m_valid);
        m_manager->remove_component<T>(*this);
}

template <typename ...Components>
inline bool Entity::has() const 
{
        return m_manager->has<Components...>(*this);
}

inline bool Entity::operator==(const Entity &rhs)
{
        return (m_manager == rhs.m_manager &&
                m_valid   == rhs.m_valid   &&
                m_id      == rhs.m_id );
}

inline bool Entity::operator!=(const Entity &rhs)
{
        return !(*this == rhs);
}

