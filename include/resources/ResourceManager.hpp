#ifndef  ETO_RESOURCE_MANAGER_HPP
#define  ETO_RESOURCE_MANAGER_HPP

#include "Resource.hpp"
#include <map>
#include <string>
#include <memory>

namespace eto {
/*
 * =====================================================================================
 *        Class:  ResourceManager
 *  Description:  Class for managing resources
 * =====================================================================================
 */
class ResourceManager
{
	public:
		ResourceManager ();                             /* constructor */
		bool addResource(std::string fileName);
		bool removeResource(ResourceId id);
		void clear();
		std::shared_ptr<Resource> getResource(ResourceId id);
	private:
		std::map<ResourceId, std::shared_ptr<Resource>> m_resources;

}; /* -----  end of class ResourceManager  ----- */

}; // namespace eto

#endif   /* ----- #ifndef ETO_RESOURCE_MANAGER_HPP  ----- */
