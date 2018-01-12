#ifndef ETO_RANGE_HPP
#define ETO_RANGE_HPP

#include <cstdint>
#include <core/Iterator.hpp>

using mask_t = std::uint64_t;
namespace eto 
{
class EntityManager;
class ComponentManager;

class Range 
{
public:
        Range(EntityManager &em, ComponentManager &cm, mask_t mask);

        Iterator begin();

        Iterator end();

        Iterator begin() const;

        Iterator end() const;

        std::size_t count() const;

private:
        EntityManager    &m_em;
        ComponentManager &m_cm;
        mask_t m_mask;
};

}

#endif 
