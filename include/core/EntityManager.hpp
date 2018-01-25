#ifndef ETO_ENTITYMANAGER_HPP
#define ETO_ENTITYMANAGER_HPP

#include <internal/Assert.hpp>
#include <core/ComponentManager.hpp>
#include <core/Range.hpp>
#include <vector>
#include <stack>
#include <map>

using std::vector;
using std::stack;

namespace eto {
namespace core {

class Entity;

/**
 *  @brief  Class for managing entites 
 *
 *  Multiple EntityManagers are allowed, however they are all share one context. 
 *  Currently, the context is limited with maximum 64 component types.
 *  @see Entity
 */
class EntityManager
{
public:
	EntityManager();

	~EntityManager();

	/**
	 *  @brief  Creates new valid entity 
	 *
	 *  Keep in mind that entities without components 
	 *  can not be retrieved by EntityManager::with<...T>()
	 *  @return A new valid entity
	 */
	Entity create();

	/**
	 *  @brief  Creates the vector of new entites
	 *
	 *  Keep in mind that entities without components 
	 *  can not be retrieved by EntityManager::with<...T>() 
	 *  @param  size Amount of the entities to create
	 *  @return std::vector<Entity> with new entities
	 */
	vector<Entity> create(size_t size);

	/**
	 *  @brief  Destroys the entity and all related components
	 *
	 *  The entity becomes invalid 
	 *  @param  e An entity to destroy
	 */
	void destroy(Entity &e);

	/**
	 *  @brief  Retrieves all entities with the specified set of components
	 *
	 *  @param  ...Components A list of component types to check
	 *  @return A range of entities with specified components
	 *  @see Range
	 */
	template <typename ...Components>
	Range with() ;

private:
	size_t find_next_id();

	std::size_t 	 m_index;
	stack<size_t>    m_free_ids;
	ComponentManager m_cmanager;
	std::mutex 	 m_mutex;
};


template <typename ...T>
Range EntityManager::with() 
{
        return Range(*this, m_cmanager, details::component_mask<T...>()); 
}

} //namespace core
} //namespace eto

#endif

