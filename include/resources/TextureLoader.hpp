#ifndef ETO_TEXTURE_LOADER
#define ETO_TEXTURE_LOADER

#include <resources/Texture.hpp>
#include <util/FileStream.hpp>
#include <stb_image/stb_image.h>

namespace eto 
{

/**
 * @brief Loader for images
 *
 * Loads an image using stb_image library
 */
class TextureLoader
{
public:
	/**
	 *  @brief  Creates Texture from given FileStream
	 *
	 *  Loads image using stb_image library and creates Texture from it.
	 *  Currently does not suppurt 1d and 3d textures.
	 *  @param  path Path to the texture
	 *  @return Shared pointer to the texture. Check the isLoaded flag to see if the texture loaded or not.
	 */
	static std::shared_ptr<Texture> load(const std::string &path);

private:
	TextureLoader ();
}; // end of class TextureLoader 

}

#endif 
