#include <resources/AssetLoader.hpp>

using namespace eto;

AssetLoader::AssetLoader() 
{
}

AssetLoader &AssetLoader::getInstance()
{
	static AssetLoader loader;
	return loader;
}
