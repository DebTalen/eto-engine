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
		 *  Loads image using stb_image library and creates Texture from it
		 *  @param  fs File stream of the texture
		 *  @return Shared pointer to the texture. If creating texture fails returns nullptr
		 */
		static SPtr<Texture> load(SPtr<FileStream> fs);

	private:
		TextureLoader ();
}; // end of class TextureLoader 

}

#endif 
