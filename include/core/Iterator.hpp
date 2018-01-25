#ifndef ETO_ITERATOR_HPP
#define ETO_ITERATOR_HPP

#include <iterator>
#include <cstddef>
#include <cstdint>
#include <atomic>

using mask_t = std::uint64_t;

namespace eto {
namespace core {

class Entity;
class EntityManager;
class ComponentManager;

/** Provides the interface for iteration through a range of entities */
class Iterator : public std::iterator<std::forward_iterator_tag, Entity>
{
public:
	Iterator(EntityManager *em, ComponentManager *cm, mask_t mask, bool begin = true);

	Iterator(const Iterator &it);

	Iterator &operator=(const Iterator &it);

	std::size_t index() const;

	Entity entity();

	Entity const entity() const;

	Iterator &operator++();

	bool operator==(Iterator const& rhs) const;

	bool operator!=(Iterator const& rhs) const;

	Entity operator*() const;

private:
	void find_next();

	EntityManager 	   *m_em;
	ComponentManager   *m_cm;
	mask_t 		    m_mask;
	std::size_t 	    m_size;
	std::atomic<size_t> m_index;
};

} // namespace core 
} // namespace eto

#endif 
