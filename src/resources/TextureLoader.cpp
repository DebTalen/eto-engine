#include <resources/TextureLoader.hpp>
#include <vector>

using namespace eto;

// currently does not suppurt 1d and 3d textures 
SPtr<Texture> TextureLoader::load(SPtr<FileStream> fs)
{
	if (! fs->isOpen() || ! fs->isReadable() )
		return nullptr;
	uchar image[fs->getSize()];
	fs->read(image, fs->getSize());

	int w, h, c;
	uchar *pData = stbi_load_from_memory(image, fs->getSize(), &w, &h, &c, 0);
	if (pData == NULL)
		return nullptr;

	Texture::TextureProps tp;
	tp.width = w;
	tp.height = h;
	// that doesn't handle 1 (grey) and 2 (grey, alpha) channels
	tp.format = (c < 4) ? Image::Format::Tex_RGB : Image::Format::Tex_RGBA; 
	// kinda should be decided considering real texture type
	tp.type = Image::Type::Tex_2D;

	std::vector<uchar> data(pData, pData + (w * h * c));
	stbi_image_free(pData);
	return Texture::create(data, tp);
}
