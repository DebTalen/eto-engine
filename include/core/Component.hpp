#ifndef ETO_COMPONENT_HPP
#define ETO_COMPONENT_HPP

#include <core/ClassTypeId.hpp>

namespace eto
{

class Component 
{
public:
	virtual ~Component() { };
	virtual void onUpdate() { }
};

template <typename T>
detail::TypeId ComponentTypeId() 
{
	return detail::ClassTypeId<Component>::GetTypeId<T>();
}

}

#endif 
