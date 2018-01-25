#ifndef ETO_SYSTEM_HPP
#define ETO_SYSTEM_HPP

namespace eto {
namespace core {

class EntityManager;

/** 
 * Base class for a system 
 *
 * All systems must derive from this class in order to be used in SystemManager
 */
struct System 
{
	virtual ~System() { }

	/**
	 *  @brief  System logic implementation
	 *
	 *  This function will be called once per every
	 *  SystemManager::update_all() call for all added systems.
	 *  @param  em Reference to entity manager
	 */
	virtual void update(EntityManager &em) = 0;
};

}
}

#endif
