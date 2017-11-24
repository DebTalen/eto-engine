#include <resources/Texture.hpp>
#include <core/Assert.hpp>

using namespace eto;


Texture::Texture(const TextureProps &tp)
	: m_tp(tp), m_loaded(0), m_error("")
{
	m_data_size = tp.width * tp.height * tp.depth;
	m_data_size *= (tp.format == Image::Format::Tex_RGB) ? 3 : 4;
}

Texture::~Texture()
{
	if (m_loaded)
		glDeleteTextures(1, &m_handle.id);
}

void Texture::write(const std::vector<uchar> &data)
{
	if (data.size() != m_data_size) {
		m_error = "Writing error: data size is invalid";
		return;
	}
	if (! m_loaded)	{
		m_loaded = load(data);
		return;
	}

	using namespace eto::Image;
	const GLvoid *pData = static_cast<const GLvoid*>(data.data());

	GLint p_id = get_prev_tex_bind();
	glBindTexture(m_tp.type, m_handle.id);
	glGetError();
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
	int err;
	if ((err = glGetError()) != GL_NO_ERROR) {
		m_error = "Error writing data to the texture: " + std::to_string(err);
		m_loaded = false;
	}
	glBindTexture(m_tp.type, p_id);
}

void Texture::read(std::vector<uchar> &data)
{
	if (m_loaded)
	{
		GLint pId = get_prev_tex_bind();
		glBindTexture(m_tp.type, m_handle.id);
		data.resize(m_data_size);
		glGetTexImage(m_tp.type, 0, m_tp.format, GL_UNSIGNED_BYTE, static_cast<GLvoid*>(data.data()));
		glBindTexture(m_tp.type, pId);
	}
	else
		m_error = "Error: texture is not loaded";
}

int Texture::load(const std::vector<uchar> &data)
{
	using namespace eto::Image;

	GLint pId = get_prev_tex_bind();
	glGenTextures(1, &m_handle.id);
	glBindTexture(m_tp.type, m_handle.id);

	glGetError(); 
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
	if (m_tp.is_mipmap)
		glGenerateMipmap(m_tp.type);
	glBindTexture(m_tp.type, pId);

	int err;
	if ( (err = glGetError()) != GL_NO_ERROR) {
		m_error = "Error loading data to the texture: " + std::to_string(err);
		return false;
	}
	return true;
}

GLint Texture::get_prev_tex_bind() 
{
	GLint pId = 0;
	if (m_tp.type == Image::Tex_1D)
		glGetIntegerv(GL_TEXTURE_BINDING_1D, &pId);
	else if (m_tp.type == Image::Tex_2D)
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &pId);
	else 
		glGetIntegerv(GL_TEXTURE_BINDING_3D, &pId);
	return pId;
}
