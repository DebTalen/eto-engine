#include <resources/Texture.hpp>
#include <core/Assert.hpp>

using namespace eto;

SPtr<Texture> Texture::create(const std::vector<uchar> &data, const TextureProps &tp)
{
	if (data.size() < 1 || tp.width < 1 || tp.height < 1)
		return nullptr;
	size_t size = tp.width * tp.height * tp.depth;
	size *= (tp.format == Image::Format::Tex_RGB) ? 3 : 4;
	if (size != data.size())
		return nullptr;

	// make_shared cannot invoke private constructor :(
	struct make_shared_enabler : public Texture {
	       	make_shared_enabler(const std::vector<uchar> &d, const TextureProps &t)
		       	: Texture(t, d) {}
       	};
	SPtr<Texture> ptr = std::make_shared<make_shared_enabler>(data, tp);		

	if (ptr->isLoaded())
		return ptr;
	return nullptr;
}

Texture::Texture(const TextureProps &tp, const std::vector<uchar> &data)
	: m_tp(tp), m_loaded(0), m_dataSize(data.size())
{
	using namespace eto::Image;

	glGenTextures(1, &m_handle.id);
	glBindTexture(m_tp.type, m_handle.id);

	const GLvoid *pData = static_cast<const GLvoid*>(data.data());
	switch (m_tp.type)
	{
		case Type::Tex_1D:
			glTexImage1D(m_tp.type, 0, m_tp.format, m_tp.width, 0, m_tp.format, GL_UNSIGNED_BYTE, pData);
			break;
		case Type::Tex_2D:
			glTexImage2D(m_tp.type, 0, m_tp.format, m_tp.width, m_tp.height, 0, m_tp.format, GL_UNSIGNED_BYTE, pData);
			break;
		case Type::Tex_3D:
			glTexImage3D(m_tp.type, 0, m_tp.format, m_tp.width, m_tp.height, m_tp.depth, 0, m_tp.format, GL_UNSIGNED_BYTE, pData);
	}
	if (m_tp.isMipmap)
		glGenerateMipmap(m_tp.type);
	m_loaded = (glGetError() == GL_NO_ERROR) ? true : false;
}

Texture::~Texture()
{
	if (m_loaded)
		glDeleteTextures(1, &m_handle.id);
}

void Texture::read(std::vector<uchar> &data)
{
	data.resize(m_dataSize);
	GLvoid *pData = static_cast<GLvoid*>(data.data());
	glGetTexImage(m_tp.type, 0, m_tp.format, GL_UNSIGNED_BYTE, pData);
}

// should also add subrectangle region to change
int Texture::write(const std::vector<uchar> &data)
{
	if (data.size() != m_dataSize)  // size of the new data should be equal to width * height * channels
		return 0;
	using namespace eto::Image;
	const GLvoid *pData = static_cast<const GLvoid*>(data.data());

	switch (m_tp.type)
	{
		case Type::Tex_1D:
			glTexSubImage1D(m_tp.type, 0, 0, m_tp.width, m_tp.format, GL_UNSIGNED_BYTE, pData);
			break;
		case Type::Tex_2D:
			glTexSubImage2D(m_tp.type, 0, 0, 0, m_tp.width, m_tp.height, m_tp.format, GL_UNSIGNED_BYTE, pData);
			break;
		case Type::Tex_3D:
			glTexSubImage3D(m_tp.type, 0, 0, 0, 0, m_tp.width, m_tp.height, m_tp.depth, m_tp.format, GL_UNSIGNED_BYTE, pData);
			break;
	}
	return 1;
}
