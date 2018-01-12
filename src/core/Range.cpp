#include <core/Range.hpp>

using namespace eto;

Range::Range(EntityManager &em, ComponentManager &cm, mask_t mask)
        : m_em(em), m_cm(cm), m_mask(mask)
{

}

Iterator Range::begin() 
{
        return Iterator(m_em, m_cm, m_mask, 1);
}

Iterator Range::end()
{
        return Iterator(m_em, m_cm, m_mask, 0);
}

Iterator Range::begin() const 
{
        return Iterator(m_em, m_cm, m_mask, 1);
}

Iterator  Range::end() const 
{
        return Iterator(m_em, m_cm, m_mask, 0);
}

std::size_t Range::count() const 
{
        std::size_t count = 0;
        for (auto it = begin(); it != end(); ++it)
                ++count;
        return count;
}

