#ifndef ETO_COMPONENTMANAGER_HPP
#define ETO_COMPONENTMANAGER_HPP

#include <thread>
#include <mutex>
#include <map>
#include <core/ComponentPool.hpp>

using eid = std::size_t; // An entity id 

namespace eto {
namespace core {

/** 
 *  Class for managing component pools 
 *  Thrad-sefe
 *  */
class ComponentManager
{
public:
	ComponentManager();

	~ComponentManager();

	/** @copydoc ComponentPool::add()  */
	template <typename T, typename ...Args>
	T &create_component(eid id, Args&& ...args);

	/** @copydoc ComponentPool::get()  */
	template <typename T>
	T &get_component(eid id) ;

	/** @copydoc ComponentPool::remove()  */
	template <typename T>
	void remove_component(eid id);

	template <typename ...Components>
	bool has(eid id) const;

	/**
	 *  @brief  Removes all components of the specified entity
	 *
	 *  @param  id An entity id to remove components
	 */
	void destroy_components(eid id);

	ComponentManager(const ComponentManager &rhs) = delete;
	ComponentManager(const ComponentManager &&rhs) = delete;
	ComponentManager& operator=(const ComponentManager &rhs) = delete;

private:
	template <typename T>
	ComponentPool<T>* get_component_pool();

	friend class Iterator;

	std::vector<mask_t> 		 m_masks;
	std::map<std::size_t, BasePool*> m_pools;
	mutable std::mutex 		 m_mutex;

	using lock_t = std::lock_guard<std::mutex>;
};

template <typename T>
inline ComponentPool<T> *ComponentManager::get_component_pool()
{
	size_t index = details::get_component_type_id<T>();
	return static_cast<ComponentPool<T>* >(m_pools[index]);
}

template <typename T, typename ...Args>
T& ComponentManager::create_component(eid id, Args&& ...args)
{
	lock_t lock(m_mutex);
	if (id >= m_masks.size()) 
		m_masks.resize(id + 1);
	mask_t comp_mask = details::component_mask<T>();
	ETO_ASSERT (! (m_masks[id] & comp_mask));   // Entity doesn't have this component
	m_masks[id] |= comp_mask;

	size_t index = details::get_component_type_id<T>();
	auto it = m_pools.find(index);
	if (it == m_pools.end() )
		m_pools[index] = new ComponentPool<T>;

	ComponentPool<T> *comp_pool = get_component_pool<T>();
	return comp_pool->add(id, std::forward<Args>(args)...);
}

template <typename T>
T& ComponentManager::get_component(eid id) 
{
	ETO_ASSERT (id < m_masks.size());
	ETO_ASSERT (m_masks[id] & details::component_mask<T>());   // Entity already have this component
	return get_component_pool<T>()->get(id);
}

template <typename T>
void ComponentManager::remove_component(eid id) 
{
	ETO_ASSERT (id < m_masks.size());
	mask_t comp_mask = details::component_mask<T>();
	ETO_ASSERT (m_masks[id] & comp_mask);   // Entity already have this component

	m_masks[id] &= ~(comp_mask);
	{
		lock_t lock(m_mutex);
		get_component_pool<T>()->remove(id);
	}
}

template <typename ...Components>
bool ComponentManager::has(eid id) const 
{
	ETO_ASSERT (id < m_masks.size());
	mask_t mask = details::component_mask<Components...>();
	return (m_masks[id] & mask)  == mask;
}

} // namespace core
} // namespace eto

#endif 
