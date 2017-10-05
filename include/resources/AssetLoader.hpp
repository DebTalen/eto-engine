#ifndef  ETO_ASSET_LOADER_HPP
#define  ETO_ASSET_LOADER_HPP

#include <resources/Resource.hpp>
#include <memory>
#include <map>

namespace eto
{

template <typename T>
using SPtr = std::shared_ptr<T>;
template <typename T>
using WPtr = std::weak_ptr<T>;

/**
 *  @brief  Class for loading resources
 *
 *  AssetLoader contain map of of a path hash value and a weak pointer to the appropriate Resource
 *  If specified
 */
class AssetLoader
{
public:
	/**
	 *  @brief  Loads asset using specified loader
	 *
	 *  Loader type must have static method "load" that takes 
	 *  at least const string reference and returns shared 
	 *  pointer to the Resource or class derived from it
	 *  @param  path Path to the asset
	 *  @param  TLoader Type of the loader to load asset
	 *  @param  args... Additional arguments to pass to TLoader::load
	 */
	template <typename TLoader, typename... Args>
	auto load(const std::string &path, Args... args)
	{
		std::size_t h = std::hash<std::string>{}(path);
		auto iter = m_loaded.find(h);
		if (iter != m_loaded.end())
		{
			if (! iter->second.expired()) 
			{
				// if resource is still in memory then return pointer to it
				// meh...
				using type = std::remove_pointer_t<decltype(TLoader::load(path, args...).get())>;
				return std::static_pointer_cast<type>(iter->second.lock());
			}
			auto pRes = TLoader::load(path, args...); // else reload it
			iter->second = std::static_pointer_cast<Resource>(pRes);
			return pRes;
		}
		auto pRes = TLoader::load(path, args...);
		m_loaded.insert( std::pair<std::size_t, WPtr<Resource>>(h, std::static_pointer_cast<Resource>(pRes)) );
		return pRes;
	}

	/* template <typename TLoader, typename... Args>
	auto reload(const std::string &path, Args... args)
	{
	}
 */	static AssetLoader &getInstance();

private:
	AssetLoader();

	std::map<std::size_t, WPtr<Resource>> m_loaded;
};

}

#endif   /* #ifndef ETO_ASSET_LOADER_HPP*/
