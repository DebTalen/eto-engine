#ifndef ETO_CLASSTYPEID_HPP
#define ETO_CLASSTYPEID_HPP

#include <atomic>

namespace eto { 
namespace detail
{
typedef std::size_t TypeId;

template <typename TBase>
class ClassTypeId
{
public:
	template <typename T> 
	static TypeId GetTypeId() 
	{
		static const TypeId id = m_nextId++;
		return id;
	}
private:
	static std::atomic<TypeId> m_nextId;
};

template <typename TBase>
std::atomic<TypeId> ClassTypeId<TBase>::m_nextId{0};

}
}

#endif 
