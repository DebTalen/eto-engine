#ifndef ETO_ENTITY
#define ETO_ENTITY

#include <core/Assert.hpp>
#include <core/Component.hpp>
#include <map>
#include <memory>

namespace eto
{
/**
 *  @brief  A general-purpose game object
 *
 *  An entity consist of components which describe it.
 */
class Entity 
{
public:
	Entity ();

	/**
	 *  @brief  Updates all components
	 *
	 *  Normally would calls each frame if the entity is active
	 */
	void update();

	/**
	 *  @brief  Adds the component 
	 *
	 *  Creates the components of the sprcific type and adds it to the entity.
	 *  The component type must be derived from Component class.
	 *  There can be only one component of the specific type per entity.
	 *  If you add a component whose type is already in the entity the old component
	 *  will be replaced with the new one.
	 *  @param  T The component type. Must be derived from the Component.
	 *  @param  Args Additional arguments for the component constructor.
	 *  @return The shared pointer to the constructed component.
	 */
	template <typename T, typename... Args>
	std::shared_ptr<T> addComponent(Args&&... args);

	/**
	 *  @brief  Retrieves the component of the specified type
	 *  @param  T The component type. Must be derived from the Component.
	 *  @return The shared pointer to the component or nullptr if no such component was found.
	 */
	template <typename T> 
	std::shared_ptr<T> getComponent() const;

	/**
	 *  @brief  Removes the component of the specified type
	 *
	 *  If entity have the component of the specified type it will be removed.
	 *  @param  T The component type. Must be derived from the Component.
	 */
	template <typename T>
	void removeComponent();

	/** Indicates whether to update the entity or not */
	bool isActive() const { return m_active; }

	/** Specified whether to update the entity or not */
	void setActive(bool v) { m_active = v; }

private:
	std::map<Component::TypeId, std::shared_ptr<Component>> m_comps;
	bool m_active;
};

template <typename T, typename... Args>
std::shared_ptr<T> Entity::addComponent(Args&&... args)
{
	ETO_ASSERT( (std::is_base_of<Component, T>::value) );
	std::shared_ptr<T> newComponent = std::make_shared<T>(std::forward<Args>(args)...);
	m_comps[T::type] = std::static_pointer_cast<Component>(newComponent);
	return newComponent;
} 

template <typename T> 
std::shared_ptr<T> Entity::getComponent() const
{
	ETO_ASSERT( (std::is_base_of<Component, T>::value) );
	auto it = m_comps.find(T::type);
	if (it != m_comps.end() )
		return std::static_pointer_cast<T>(it->second);
	return nullptr;
}


template <typename T>
void Entity::removeComponent()
{
	ETO_ASSERT( (std::is_base_of<Component, T>::value) );
	auto it = m_comps.find(T::type);
	if (it != m_comps.end() )
		m_comps.erase(it);
}


}

#endif 
