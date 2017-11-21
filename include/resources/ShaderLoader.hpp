#ifndef ETO_SHADER_LOADER
#define ETO_SHADER_LOADER

#include <resources/Shader.hpp>
#include <util/FileStream.hpp>

#include <memory>

namespace eto
{

/**
 * @brief Loader for shaders
 */
class ShaderLoader
{
public:
	/**
	 *  @brief  Loads shader from givet FileStream
	 *
	 *  @param  path Path to the shader source file
	 *  @return Shared pointer to the loaded Shader. If creation fails is_compiled flag is set to 0.
	 *          Use get_error_message function to retrieve error.
	 */
	static std::shared_ptr<Shader> load(const std::string &path, ShaderType type)
	{
		FileStream fs(path);
		if(! fs.is_open())
		{
			std::shared_ptr<Shader> pFail = std::make_shared<Shader>(type);
			pFail->m_error = "Error: Unable to load file " + path;
			return pFail;
		}
		char *buffer = new char[fs.get_size() + 1];
		fs.read(buffer, fs.get_size());
		buffer[fs.get_size()] = '\0';

		std::shared_ptr<Shader> pShader = std::make_shared<Shader>(type);
		pShader->compile(std::string(buffer));
		delete[] buffer;
		return pShader;
	}

private:
	ShaderLoader ();
}; // end of class TextureLoader

}

#endif
