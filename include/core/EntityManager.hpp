#ifndef ETO_ENTITYMANAGER_HPP
#define ETO_ENTITYMANAGER_HPP

#include <core/Assert.hpp>
#include <core/ComponentManager.hpp>
#include <core/Range.hpp>
#include <vector>
#include <stack>
#include <map>

using std::vector;
using std::stack;

namespace eto 
{

class Entity;

/**
 *  @brief  Helper class for managing entites and their components
 *
 *  Multiple EntityManagers are allowed, however they are all share one context. 
 *  Right now the context is limited with maximum 64 component types.
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
	 *  can not be retrieved by Entity::has<T>() function
	 *  @return The new valid entity
	 */
	Entity create();

	/**
	 *  @brief  Creates the vector of new entites
	 *
	 *  Keep in mind that entities without components 
	 *  can not be retrieved by Entity::has<T>() function
	 *  @param  size Amount of the entities to create
	 *  @return std::vector<Entity> with new entities
	 */
	vector<Entity> create(size_t size);

	/**
	 *  @brief  Destroys the entity and all related components
	 *
	 *  The entity becomes invalid 
	 *  @param  e The entity to destroy
	 */
	void destroy(Entity &e);

	/**
	 *  @brief  Retrieves all entities with the specified set of components
	 *
	 *  @param  ...Components List of the component types to check
	 *  @return The range of entities with specified components
	 *  @see Range
	 */
	template <typename ...Components>
	Range with() ;
private:
	friend class Entity;
	friend class Iterator;

	template <typename T, typename ...Args>
	T &create_component(const Entity &e, Args&& ...args);

	template <typename T>
	T &get_component(const Entity &e);

	template <typename T>
	void remove_component(const Entity &e);

	template <typename T>
	ComponentManager<T>* get_component_manager();

	template <typename ...Components>
	bool has(const Entity &e) const;

	size_t find_next_id();

	std::map<size_t, BaseManager*> m_managers;
	vector<mask_t>       m_masks;
	stack<size_t>        m_free_ids;
	size_t 		     m_index;
};

}

#include <core/EntityManager.i>

#endif

