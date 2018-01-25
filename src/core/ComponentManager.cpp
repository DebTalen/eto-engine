#include <core/ComponentManager.hpp>

using namespace eto::core;

ComponentManager::ComponentManager()
{
}

ComponentManager::~ComponentManager()
{
	for (auto it : m_pools)
		if (it.second)
			delete it.second;
}

void ComponentManager::destroy_components(eid id)
{
	lock_t lock(m_mutex);
	if(id < m_masks.size()) // entity has components
	{
		if (m_masks[id])
		{
			for (int i = 0; i < 64; ++i)
				if ((m_masks[id] >> i) & 1)
					m_pools[i]->remove(id);
		}
		m_masks[id] = 0;
	}
}

