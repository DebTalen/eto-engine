#include <core/EntityManager.hpp>
#include <core/ComponentManager.hpp>
#include <core/Entity.hpp>

using namespace eto::core;

EntityManager::EntityManager() 
	: m_index(0)
{

}

EntityManager::~EntityManager()
{

}

Entity EntityManager::create() 
{
	return { *this, m_cmanager, find_next_id() };
}

std::vector<Entity> EntityManager::create(size_t amount)
{
        std::vector<Entity> arr;
        arr.reserve(amount);
        for (size_t i = 0; i < amount; ++i)
        {
                Entity e { *this, m_cmanager, find_next_id() };
                arr.push_back( std::move(e) );
        }
        return arr;
}

size_t EntityManager::find_next_id()
{
        std::lock_guard<std::mutex> lk(m_mutex);
        if (m_free_ids.size())
        {
                size_t id = m_free_ids.top();
                m_free_ids.pop();
                return id;
        }
        return m_index++;
}

void EntityManager::destroy(Entity &e)
{
        std::lock_guard<std::mutex> lk(m_mutex);
        if (! e.m_valid)
                return;
        ETO_ASSERT(e.m_id < m_index);
        m_cmanager.destroy_components(e.m_id);
        m_free_ids.push(e.m_id);
        e.m_valid = 0;
}
