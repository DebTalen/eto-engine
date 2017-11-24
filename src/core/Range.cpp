#include <core/Range.hpp>

using namespace eto;

Range::Range(EntityManager &manager, mask_t mask)
        : m_manager(&manager), m_mask(mask)
{

}

Range::iterator Range::begin() 
{
        return Iterator(*m_manager, m_mask, 1);
}

Range::iterator Range::end()
{
        return Iterator(*m_manager, m_mask, 0);
}

Range::iterator Range::begin() const 
{
        return Iterator(*m_manager, m_mask, 1);
}

Range::iterator  Range::end() const 
{
        return Iterator(*m_manager, m_mask, 0);
}

std::size_t Range::count() const 
{
        std::size_t count = 0;
        for (auto it = begin(); it != end(); ++it)
                ++count;
        return count;
}

