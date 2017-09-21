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
		 *  @param  fs FileStream that contain shader source code
		 *  @return Shared pointer to the loaded Shader. If creation fails isCompiled flag is set to 0.
		 *          Use getErrorMessage function to retrieve error.
		 */
		static std::shared_ptr<Shader> load(std::shared_ptr<FileStream> fs, ShaderType type)
		{
			char buffer[fs->getSize()];
			fs->read(buffer, fs->getSize());
			buffer[fs->getSize()] = '\0';
			
			std::shared_ptr<Shader> pShader = std::make_shared<Shader>(type);
			pShader->compile(std::string(buffer));
			return pShader;
		}

	private:
		ShaderLoader ();
}; // end of class TextureLoader 

}

#endif 
