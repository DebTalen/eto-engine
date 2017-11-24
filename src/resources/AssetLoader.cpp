#include <resources/AssetLoader.hpp>

using namespace eto;

AssetLoader::AssetLoader() 
{
}

AssetLoader &AssetLoader::get_instance()
{
	static AssetLoader loader;
	return loader;
}
