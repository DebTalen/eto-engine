#ifndef ETO_RESOURCE_HPP
#define ETO_RESOURCE_HPP

#include <string>

namespace eto
{
	typedef int ResourceId;

/*
 * =====================================================================================
 *        Class:  Resource
 *  Description:  Base class for all resources
 * =====================================================================================
 */
class Resource
{
	public:
		Resource();
		virtual void load() = 0;
		virtual void unLoad() = 0;

		ResourceId getId() { return m_id; }
		bool isLoaded() { return m_loaded; }

		virtual ~Resource();
	protected:
		ResourceId  m_id;
		bool 	    m_loaded;
		std::string m_fileName;

}; /* -----  end of class Resource  ----- */
}; // namespace eto

#endif // ETO_RESOURCE_HPP
