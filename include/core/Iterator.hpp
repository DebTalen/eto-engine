#ifndef ETO_ITERATOR_HPP
#define ETO_ITERATOR_HPP

#include <iterator>
#include <cstddef>
#include <cstdint>
using mask_t = std::uint64_t;

namespace eto 
{
class Entity;
class EntityManager;

class Iterator : public std::iterator<std::forward_iterator_tag, Entity>
{
public:
	Iterator(EntityManager &manager, mask_t mask, bool begin = true);

	Iterator(const Iterator &it) = default;

	Iterator &operator=(const Iterator &it) = default;

	std::size_t index() const;

	Entity entity();

	Entity const entity() const;

	Iterator &operator++();

	bool operator==(Iterator const& rhs) const;

	bool operator!=(Iterator const& rhs) const;

	Entity operator*() const;

private:
	void find_next();

	EntityManager *m_manager;
	std::size_t m_size;
	std::size_t m_index;
	mask_t m_mask;
};


}

#endif 
