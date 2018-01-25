#include <core/SystemManager.hpp>
#include <core/EntityManager.hpp>

using namespace eto::core;

SystemManager::SystemManager(EntityManager &em)
	: m_systems(5, nullptr), m_manager(em),
       	  m_pool(std::thread::hardware_concurrency() - 1)
{
}

SystemManager::~SystemManager()
{
	for (System *s : m_systems)
		if (s) delete s;
}

// Looks like I need to hardcode specific system's ids, so that I can use them in parallel.
// So, for now, it's just squentual implementation
void SystemManager::update_all()
{
	for (System *s : m_systems)
		if (s) s->update(m_manager);
}
