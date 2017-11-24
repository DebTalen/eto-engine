#ifndef ETO_RANGE_HPP
#define ETO_RANGE_HPP

#include <cstdint>
#include <core/Iterator.hpp>

using mask_t = std::uint64_t;
namespace eto 
{

class EntityManager;
class Range 
{
public:
        using iterator = Iterator;

        Range(EntityManager &manager, mask_t mask);

        iterator begin();
        iterator end();
        iterator begin() const;
        iterator end() const;

        std::size_t count() const;

private:
        EntityManager *m_manager;
        mask_t         m_mask;
};

}

#endif 
