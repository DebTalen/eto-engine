#ifndef ETO_COMPONENTMANAGER_HPP
#define ETO_COMPONENTMANAGER_HPP

#include <vector>
#include <deque>
#include <cstdint>
#include <core/Assert.hpp>

using std::vector;
using mask_t = std::uint64_t;
using eid = std::size_t;

namespace eto 
{

namespace details
{

inline size_t& get_next_type_id() 
{
	static size_t next_id = 0;
	return next_id;
}

template <typename T>
inline size_t get_component_type_id() 
{
	static size_t id = get_next_type_id()++;
	ETO_ASSERT( id < 65 );
	return id;
}

template <typename T>
inline mask_t component_mask()
{
        return (1UL << get_component_type_id<T>());
}

template <typename C1, typename C2, typename ...Args>
inline mask_t component_mask() 
{
        return ((1UL << get_component_type_id<C1>()) | component_mask<C2, Args...>());
}

}

class BaseManager
{
public:
	virtual ~BaseManager() { } 
	virtual void remove(size_t id) = 0;
};

/** Contain all components of the specific type  */
template <typename T>
class ComponentManager: public BaseManager
{
public:
	/**
	 *  @brief  Constuctor
	 *  @param  reserve The number of elements to reserve memory for
	 */
	ComponentManager(size_t reserve = 64);

	/**
	 *  @brief  Creates new element for the given id with specified arguments
	 *
	 *  There can be only one component per id
	 *  @param  id Entity id to which to add component
	 *  @return Reference to the created component
	 */
	template <typename ...Args>
	T& add(size_t id, Args&& ...args);

	/**
	 *  @brief  Retrives component for the given id
	 *
	 *  The specified id must have component of this type
	 *  @param  id Entity id to get component form
	 *  @return Reference to the comonent
	 */
	T& get(size_t id) ;

	/**
	 *  @brief  Removes component for the given id
	 *
	 *  The specified id must have component of this type
	 *  @param  id Entity id to remove component from
	 */
	virtual void remove(size_t id) override;

private:
	ComponentManager(const ComponentManager<T> &rhs);

	ComponentManager<T>& operator=(const ComponentManager<T> &rhs);

	int next_index();

	vector<T> 	m_data;
	vector<int> 	m_indexes;
	std::deque<int> m_free;
};

template <typename T>
ComponentManager<T>::ComponentManager(size_t reserve)
	: m_data(reserve), m_indexes(reserve, -1)
{
	for (size_t i = 0; i < reserve; ++i)
		m_free.push_back(i);
}

template <typename T>
template <typename ...Args>
T& ComponentManager<T>::add(size_t id, Args&& ...args)
{
	if (id >= m_indexes.size())
		m_indexes.resize(id + 1 + (m_indexes.size() / 3), -1);
	ETO_ASSERT( m_indexes[id] == -1 ); // entity with this id shouldn't have T component yet

	m_indexes[id] = next_index();  // find first free memory block or allocate new if there is no free
	m_data[m_indexes[id]] = T{ std::forward<Args&&>(args)... };
	return m_data[m_indexes[id]];
}

template <typename T>
T& ComponentManager<T>::get(size_t id) 
{
	ETO_ASSERT(id < m_indexes.size()); // existing id
	ETO_ASSERT(m_indexes[id] != -1);   // memory block is not free
	return m_data[m_indexes[id]];
}

template <typename T>
void ComponentManager<T>::remove(size_t id) 
{
	ETO_ASSERT(id < m_indexes.size()); // existing id
	ETO_ASSERT(m_indexes[id] != -1);   // memory block is not free
	m_free.push_front(m_indexes[id]);  // memory block is now free
	m_indexes[id] = -1; 		   // no component for this id
}

template <typename T>
int ComponentManager<T>::next_index() 
{
	if (m_free.size())
	{
		int new_id = m_free.front();
		m_free.pop_front();
		return new_id;
	}

	// if there is no free block allocate new
	int new_id = m_data.size();
	int new_size = new_id + (new_id / 3); // plus 1/3 of current size 
	for (int i = new_id + 1; i < new_size; ++i)
		m_free.push_back(i);
	m_data.resize(new_size);
	return new_id;
}

}


#endif 
