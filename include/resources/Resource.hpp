#ifndef ETO_RESOURCE_HPP
#define ETO_RESOURCE_HPP

#include <string>

namespace eto
{

/**
*  @brief  Represents GPU resource
*/
struct ResourceHandle
{
	typedef uint ResId;
	static const ResId NULL_ID = 0;

	ResourceHandle() : id(NULL_ID) {}
	bool isNull() const { return (id == NULL_ID); }

	ResId id; 	/*!< ResId is used to address to the GPU resource*/
}; 

/**
 *  @brief  Base class for resources
 */
class Resource
{
public:
	typedef std::int_least64_t Id;
        virtual ~Resource() {}
        
	Resource(): m_id(++sNextId) {}

	Id getId() const { return m_id; }

protected:
	ResourceHandle m_handle;
private:
	static Id sNextId;
	Id        m_id;         /*!< Id is used to uniquely identify the resource */

}; /* end of class Resource */

}

#endif // ETO_RESOURCE_HPP
