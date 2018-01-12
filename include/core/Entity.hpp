#ifndef ETO_ENTITY_HPP
#define ETO_ENTITY_HPP
#include <cstddef>
#include <core/ComponentManager.hpp>

namespace eto 
{

class EntityManager;

/**
 *  @brief  Represents every game object 
 *
 *  Entities consist of the components. 
 *  The component can be any type from build-in types to user-defined ones.
 *  No inhertance needed. If the component is a class, than it must have a default constructor or 
 *  a constructor with default values to all of its arguments.
 *  Entities must be created via EntityManager
 *  Coping entity is currently unavaliable 
 *  @see EntityManager
 */
class Entity
{
public:
	Entity(const Entity &&rhs);

	/**
	 *  @brief  Adds component of the specified type to the entity
	 *
	 *  Only one component per type is allowed.
	 *  Use Entity::has<C>() to check whether entity has the component of type C or not.
	 *  @param  T Type of the component to add
	 *  @param  ...Args Optional arguments to initialize the component
	 *  @return Reference to the created component
	 */
	template <typename T, typename ...Args>
	T& add(Args&& ...args);

	/**
	 *  @brief  Retrieves the specified component from the entity
	 *
	 *  Entity must have the component of specified type.
	 *  Use Entity::has<C>() to check whether entity has the component of type C or not.
	 *  @param  T Type of the component to retrieve
	 *  @return Reference to the component
	 */
	template <typename T>
	T& get() const;

	/**
	 *  @brief  Removes the specified component from the entity
	 *
	 *  Entity must have the component of specified type.
	 *  Use Entity::has<C>() to check whether entity has the component of type C or not.
	 *  @param  T Type of the component to remove
	 */
	template <typename T>
	void remove();

	/**
	 *  @brief  Checks the presence of the specified components
	 *
	 *  @param  ...Components List of the component types to check
	 *  @return true if the entity has all of these components, otherwise false
	 */
	template <typename ...Components>
	bool has() const;

	/**
	 *  @brief  Checks the validity of the entity 
	 *
	 *  Do not use invalid entities, it will leads to the assertions errors
	 *  @return true if the entity is valid, otherwise false
	 */
	bool is_valid() const { return m_valid; } 

	/**
	 *  @brief  Destroys the entity and all related components
	 *
	 *  The entity becomes invalid 
	 */
	void destroy();

	bool operator==(const Entity &rhs) const;
	bool operator!=(const Entity &rhs) const;

private:
	Entity(EntityManager &emanager, ComponentManager &cmanager, eid id); 

	Entity(const Entity &rhs) = delete;

	Entity& operator=(const Entity &rhs) = delete;

	Entity(EntityManager &&, ComponentManager &&, eid) = delete; // prevents references to temporary objects

	eid  m_id;
	bool m_valid;
	EntityManager 	 &m_emanager;
	ComponentManager &m_cmanager;

	friend class EntityManager;
	friend class Iterator;
};

template <typename T, typename ...Args>
inline T& Entity::add(Args&& ...args)
{
        ETO_ASSERT (m_valid);
        return m_cmanager.create_component<T>(m_id, std::forward<Args>(args)...);
}

template <typename T>
inline T& Entity::get() const 
{
        ETO_ASSERT (m_valid);
        return m_cmanager.get_component<T>(m_id);
}

template <typename T>
inline void Entity::remove() 
{ 
        ETO_ASSERT (m_valid);
        m_cmanager.remove_component<T>(m_id);
}

template <typename ...Components>
inline bool Entity::has() const 
{
        return m_cmanager.has<Components...>(m_id);
}

}

#endif
