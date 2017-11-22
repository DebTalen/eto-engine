#ifndef ETO_ENTITY_HPP
#define ETO_ENTITY_HPP
#include <cstddef>

namespace eto 
{

class EntityManager;
class Entity
{
public:
	using eid = size_t;

	Entity(EntityManager &manager, eid id);

	Entity(Entity &&rhs);

	template <typename T, typename ...Args>
	T& add(Args&& ...args);

	template <typename T>
	T& get() const;

	template <typename T>
	void remove();

	template <typename ...Components>
	bool has() const;

	bool is_valid() const { return m_valid; } 

	void destroy();

	bool operator==(const Entity &rhs);
	bool operator!=(const Entity &rhs);
private:
	Entity(const Entity &rhs);

	Entity& operator=(const Entity &rhs);

	EntityManager *m_manager;
	eid  m_id;
	bool m_valid;

	friend class EntityManager;
	friend class Iterator;
};

#include <core/Entity.i>

}

#endif
