#include <core/Entity.hpp>

using namespace eto;

inline EntityManager::EntityManager() 
	: m_index(0)
{

}

inline EntityManager::~EntityManager()
{
	for (auto it : m_managers)
		if (it.second)
			delete it.second;
}

inline Entity EntityManager::create() 
{
	return { *this, find_next_id() };
}

inline std::vector<Entity> EntityManager::create(size_t amount)
{
        std::vector<Entity> arr;
        arr.reserve(amount);
        for (size_t i = 0; i < amount; ++i)
        {
                Entity e { *this, find_next_id() };
                arr.push_back( std::move(e) );
        }
        return arr;
}

inline size_t EntityManager::find_next_id()
{
        if (m_free_ids.size())
        {
                size_t id = m_free_ids.top();
                m_free_ids.pop();
                return id;
        }
        return m_index++;
}

inline void EntityManager::destroy(Entity &e)
{
        if (! e.m_valid)
                return;
        ETO_ASSERT(e.m_id < m_index);
        if(e.m_id < m_masks.size()) // entity has components
        {
                if (m_masks[e.m_id])
                {
                        for (int i = 0; i < 64; ++i)
                                if ((m_masks[e.m_id] >> i) & 1)
                                        m_managers[i]->remove(e.m_id);
                }
                m_masks[e.m_id] = 0;
        }
        m_free_ids.push(e.m_id);
        e.m_valid = 0;
}

template <typename ...Components>
inline bool EntityManager::has(const Entity &e) const 
{
        ETO_ASSERT (e.m_id < m_masks.size());
        mask_t mask = details::component_mask<Components...>();
        return (m_masks[e.m_id] & mask)  == mask;
}

template <typename T>
inline ComponentManager<T> *EntityManager::get_component_manager()
{
	size_t index = details::get_component_type_id<T>();
        auto it = m_managers.find(index);
        if (it == m_managers.end() )
	        m_managers[index] = new ComponentManager<T>;
	return static_cast<ComponentManager<T>* >(m_managers[index]);
}

template <typename T, typename ...Args>
inline T& EntityManager::create_component(const Entity &e, Args&& ...args)
{
        ETO_ASSERT (e.m_id < m_index);
        if (e.m_id >= m_masks.size()) 
                m_masks.resize(e.m_id + 1);
        mask_t comp_mask = details::component_mask<T>();
        ETO_ASSERT (! (m_masks[e.m_id] & comp_mask));   // Entity doesn't have this component
        m_masks[e.m_id] |= comp_mask;

	ComponentManager<T> *comp_manager = get_component_manager<T>();
        return comp_manager->add(e.m_id, std::forward<Args>(args)...);
}

template <typename T>
inline T& EntityManager::get_component(const Entity &e)
{
        ETO_ASSERT (e.m_id < m_index);
        ETO_ASSERT (e.m_id < m_masks.size());

        ETO_ASSERT (m_masks[e.m_id] & details::component_mask<T>());   // Entity already have this component
        return get_component_manager<T>()->get(e.m_id);
}

template <typename T>
inline void EntityManager::remove_component(const Entity &e) 
{
        ETO_ASSERT (e.m_id < m_index);
        ETO_ASSERT (e.m_id < m_masks.size());
        mask_t comp_mask = details::component_mask<T>();
        ETO_ASSERT (m_masks[e.m_id] & comp_mask);   // Entity already have this component

        m_masks[e.m_id] &= ~(comp_mask);
        get_component_manager<T>()->remove(e.m_id);
}

template <typename ...T>
inline Range EntityManager::with() 
{
        return Range(*this, details::component_mask<T...>()); 
}



