#ifndef  ETO_TEXTURE
#define  ETO_TEXTURE

#include <resources/Resource.hpp>
#include <memory>
#include <vector>
#include <glad/glad.h>

typedef unsigned char uchar;
typedef unsigned int uint;
namespace eto
{

namespace Image
{
	enum Type  {
		Tex_1D = GL_TEXTURE_1D,
		Tex_2D = GL_TEXTURE_2D,
		Tex_3D = GL_TEXTURE_3D
	};

	enum Usage {
		Static  = GL_STATIC_DRAW,
		Dynamic = GL_DYNAMIC_DRAW,
		Stream  = GL_STREAM_DRAW
	};

	enum Format {
		Tex_RGB  = GL_RGB,
		Tex_RGBA = GL_RGBA,
	};
}

/**
*  @brief  Represents texture in memory
*/
class Texture: public Resource
{
public:
	/** Contain texture propreties */
	struct TextureProps
	{
		Image::Type   type = Image::Type::Tex_2D;
		Image::Usage  usage = Image::Usage::Dynamic;
		Image::Format format = Image::Format::Tex_RGB;
		uint   	      width = 1;
		uint          height = 1;
		uint	      depth = 1;
		bool          is_mipmap = 1;
		TextureProps() {}
	};

	explicit Texture(const TextureProps &tp = {});

	~Texture();

	/**
	 *  @brief  Writes data from buffer and transfers it to internal GPU texture
	 *  
	 *  This will override existing texture data 
	 *  @param  data Buffer that contains data
	 */
	void write(const std::vector<uchar> &data); // should also add subrectangle region to change

	/**
	 *  @brief Reads internal GPU texture data to the specified buffer
	 *  @param  buff Buffer to write
	 */
	void read(std::vector<uchar> &data);

	uint get_width() const { return m_tp.width; }
	uint get_height() const { return m_tp.height; }
	uint get_depth() const { return m_tp.depth; }
	uint get_components() const { return (m_tp.format == Image::Format::Tex_RGB) ? 3 : 4; }
	bool is_loaded() const { return m_loaded; }
	bool is_mipmaped() const { return m_tp.is_mipmap; }
	std::size_t get_data_size() const { return m_data_size; }
	std::string get_error_message() const { return m_error; }
private:
	Texture(const Texture &rhs) = delete;
	Texture &operator= (const Texture &rhs) = delete;

	int load(const std::vector<uchar> &data); 
	GLint get_prev_tex_bind();

	friend class Renderer;
	friend class TextureLoader;
	void set_error_message(const std::string &error) { m_error = error; }

	TextureProps m_tp;
	bool	     m_loaded;
	std::size_t  m_data_size;
	std::string  m_error;
};
}

#endif   // #ifndef ETO_TEXTURE
