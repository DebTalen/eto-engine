#ifndef ETO_SYSTEM_MANAGER_HPP
#define ETO_SYSTEM_MANAGER_HPP

#include <mutex>
#include <vector>

#include <internal/Assert.hpp>
#include <internal/ThreadPool.hpp>
#include <core/System.hpp>

namespace eto {
namespace core {

namespace details
{
inline std::size_t& get_next_system_id() 
{
        static size_t next_system_id = 0;
        return next_system_id;
}

template <typename T>
inline std::size_t get_system_type_id()
{
        static std::size_t system_id = get_next_system_id()++;
        return system_id;
}

}

class EntityManager;

/**
 *  @brief  Class for managing systems execution 
 *
 *  Internally uses thread pool to run predefined systems in concrete order.
 *  Currently, all added systems are executed sequentially in order of addition.
 */
class SystemManager 
{
public:
	SystemManager(EntityManager &em);

	~SystemManager();

	/**
	 *  @brief  Adds a system T
	 *
	 *  System T must be derived from core::System class
	 *  @param  ...args Arguments to construct a system T
	 */
	template <typename T, typename ...Args>
	void add(Args&& ...args);

	/**
	 *  @brief  Removes a system T
	 *
	 *  System T must be derived from core::System class
	 *  System T must be added earlier
	 */
	template <typename T>
	void remove();

	/**
	 *  @brief  Synchronously updates all added systems
	 *
	 *  Calls the core::System::update() on each system, in order of addition.
	 *  Function is returned after all systems have been executed.
	 */
	void update_all();

	SystemManager(const SystemManager&) = delete;
	SystemManager& operator=(const SystemManager&) = delete;
private:

	std::vector<System*> m_systems;
	EntityManager 	    &m_manager;
	std::mutex 	     m_mutex;
	internal::ThreadPool m_pool;
};


template <typename T, typename ...Args>
void SystemManager::add(Args&& ...args)
{
	static_assert(std::is_base_of<System, T>::value == true, 
		      "Invalid system type in function SystemManager::add<T,Args...>");

	size_t id = details::get_system_type_id<T>();
	if (id >= m_systems.size())
		m_systems.resize(id + 1, nullptr);
	else
		ETO_ASSERT(m_systems[id] == nullptr); // System must not be added earlier

	m_systems[id] = new T{ std::forward<Args>(args)... };
}

template <typename T>
void SystemManager::remove()
{
	static_assert(std::is_base_of<System, T>::value == true, 
		      "Invalid system type in function SystemManager::add<T,Args...>");

	size_t id = details::get_system_type_id<T>();
	ETO_ASSERT(id < m_systems.size());
	ETO_ASSERT(m_systems[id]); // System must be added earlier
	delete m_systems[id];
	m_systems[id] = nullptr;
}

}
}

#endif 
