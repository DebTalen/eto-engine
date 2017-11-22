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
class EntityManager
{
public:
	EntityManager();

	~EntityManager();

	Entity create();

	vector<Entity> create(size_t size);

	void destroy(Entity &e);

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

