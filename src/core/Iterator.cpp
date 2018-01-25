#include <core/Iterator.hpp>
#include <core/EntityManager.hpp>
#include <core/Entity.hpp>

using namespace eto::core;

Iterator::Iterator(EntityManager *em, ComponentManager *cm, mask_t mask, bool begin) 
	: m_em(em), m_cm(cm), m_mask(mask), m_index(0)
{
        m_size = m_cm->m_masks.size();
        if (!begin) m_index = m_size;
        find_next();
}

size_t Iterator::index() const
{
        return m_index;
}

inline void Iterator::find_next() 
{
        while ((m_index < m_size) && (m_cm->m_masks[m_index] & m_mask) != m_mask)
                ++m_index;
}

Entity Iterator::entity()
{
        return { *m_em, *m_cm, m_index };
}

const Entity Iterator::entity() const  
{
        return { *m_em, *m_cm,  m_index };
}

Iterator::Iterator(const Iterator &it)
	: m_em(it.m_em), m_cm(it.m_cm),
	  m_mask(it.m_mask), m_size(it.m_size),
	  m_index(it.m_index.load(std::memory_order_relaxed))
{

}

Iterator& Iterator::operator=(const Iterator &it)
{
	if (*this != it)
	{
		m_em = it.m_em;
		m_cm = it.m_cm;
		m_mask = it.m_mask;
		m_size = it.m_size;
		m_index = it.m_index.load(std::memory_order_relaxed);
	}
	return *this;
}

Iterator &Iterator::operator++() 
{
	++m_index;
	find_next();
	return *this;
}

bool Iterator::operator==(Iterator const &rhs) const
{
	return index() == rhs.index();
}

bool Iterator::operator!=(Iterator const &rhs) const
{
	return !(*this == rhs);
}

Entity Iterator::operator*() const
{
	return entity();
}

