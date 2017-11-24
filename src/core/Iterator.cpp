#include <core/Iterator.hpp>
#include <core/EntityManager.hpp>

using namespace eto;

Iterator::Iterator(EntityManager &manager, mask_t mask, bool begin) :
        m_manager(&manager),  m_index(0), m_mask(mask)
{
        m_size = m_manager->m_masks.size();
        if (!begin) m_index = m_size;
        find_next();
}

size_t Iterator::index() const
{
        return m_index;
}

inline void Iterator::find_next() 
{
        while ((m_index < m_size) && (m_manager->m_masks[m_index] & m_mask) != m_mask)
                ++m_index;
}

Entity Iterator::entity()
{
        return { *m_manager, m_index };
}

const Entity Iterator::entity() const  
{
        return { *m_manager, m_index };
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

