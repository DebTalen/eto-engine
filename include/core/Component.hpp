#ifndef ETO_COMPONENT_HPP
#define ETO_COMPONENT_HPP

namespace eto
{

class Component 
{
public:
	typedef unsigned long TypeId;
	virtual ~Component() { };
	virtual void onUpdate() { }
	virtual TypeId getType() const = 0;
};

template <typename T>
class ComponentBase : public Component
{
public:
	static TypeId type;
	virtual ~ComponentBase() { };
	TypeId getType() const override { return T::type; }
};

static Component::TypeId nextType = 0;
template <typename T>
Component::TypeId ComponentBase<T>::type(nextType++);

}

#endif 
