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

	/**
	 *  @brief  Writes data from buffer and transfers it to internal GPU texture
	 *  
	 *  It overrides existing texture data 
	 *  @param  data Buffer that contains data
	 *  @return 0 if failed otherwise 1
	 */
	int write(const std::vector<uchar> &data);

	/**
	 *  @brief Reads internal GPU texture data to the specified buffer
	 *  @param  buff Buffer to write
	 */
	void read(std::vector<uchar> &data);

	uint getWidth() const { return m_tp.width; }
	uint getHeight() const { return m_tp.height; }
	uint getComponents() const { return (m_tp.format == Image::Format::Tex_RGB) ? 3 : 4; }
	bool isLoaded() const { return m_loaded; }
	std::size_t getDataSize() const { return m_dataSize; }

	/**
	 *  @brief  Creates new Texture from sprcified data
	 *  @param  data Texture data
	 *  @param  tp Texture propreties
	 *  @return Shared pointer to the new Texture or nullptr if creations fails
	 */
	static SPtr<Texture> create(const std::vector<uchar> &data, const TextureProps &tp = {});
	~Texture();
private:
	// temporary, should be replaced by Renderer
	friend class Model;
	explicit Texture(const TextureProps &tp, const std::vector<uchar> &data);
	//explicit Texture(const TextureProps &tp);

	TextureProps m_tp;
	bool	     m_loaded;
	std::size_t  m_dataSize;
};
}

#endif   // #ifndef ETO_TEXTURE
