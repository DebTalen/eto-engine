#include <resources/TextureLoader.hpp>
#include <vector>

using namespace eto;

SPtr<Texture> TextureLoader::load(const std::string &path)
{
	if (path.empty()) {
		SPtr<Texture> fail = std::make_shared<Texture>();
		fail->setErrorMessage("Invalid file name");
		return fail;
	}

	int w, h, c;
	stbi_set_flip_vertically_on_load(true);
	uchar *pData = stbi_load(path.c_str(), &w, &h, &c, 0);
	if (pData == NULL) {
		SPtr<Texture> fail = std::make_shared<Texture>();
		fail->setErrorMessage("Invalid file name: " + path);
		return fail;
	}

	Texture::TextureProps tp;
	tp.width = w;
	tp.height = h;
	// that doesn't handle 1 (grey) and 2 (grey, alpha) channels
	tp.format = (c < 4) ? Image::Format::Tex_RGB : Image::Format::Tex_RGBA; 
	// kinda should be decided considering real texture type
	tp.type = Image::Type::Tex_2D;

	std::vector<uchar> data(pData, pData + (w * h * c));
	stbi_image_free(pData);
	SPtr<Texture> texture = std::make_shared<Texture>(tp);
	texture->write(data);
	return texture;
}
