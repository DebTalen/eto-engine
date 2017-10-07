#ifndef  ETO_TEXTURE
#define  ETO_TEXTURE

#include <resources/Resource.hpp>
#include <memory>
#include <vector>
#include <glad/glad.h>

namespace eto
{
template <typename T>
using SPtr = std::shared_ptr<T>;
typedef unsigned char uchar;

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
		bool          isMipmap = 1;
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

	uint getWidth() const { return m_tp.width; }
	uint getHeight() const { return m_tp.height; }
	uint getDepth() const { return m_tp.depth; }
	uint getComponents() const { return (m_tp.format == Image::Format::Tex_RGB) ? 3 : 4; }
	bool isLoaded() const { return m_loaded; }
	bool isMipmaped() const { return m_tp.isMipmap; }
	std::size_t getDataSize() const { return m_dataSize; }
	std::string getErrorMessage() const { return m_error; }
private:
	Texture(const Texture &rhs) = delete;
	Texture &operator= (const Texture &rhs) = delete;

	int load(const std::vector<uchar> &data); 
	GLint getPrevTexBind();

	// temporary, should be replaced by Renderer
	friend class Model;
	friend class TextureLoader;
	void setErrorMessage(const std::string &error) { m_error = error; }

	TextureProps m_tp;
	bool	     m_loaded;
	std::size_t  m_dataSize;
	std::string  m_error;
};
}

#endif   // #ifndef ETO_TEXTURE
