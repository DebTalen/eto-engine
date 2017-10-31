#ifndef ETO_GPUSHADER_HPP
#define ETO_GPUSHADER_HPP

#include <glad/glad.h>
#include <resources/Resource.hpp>
#include <string>

namespace eto 
{
// Enumerates all possible Shader types
enum ShaderType
{
	VertexShader   = GL_VERTEX_SHADER,
	GeometryShader = GL_GEOMETRY_SHADER,
	FragmentShader = GL_FRAGMENT_SHADER
};

/**
 * @brief Represents GLSL shader
 */
class Shader : public Resource
{
public:
	Shader(ShaderType type);
	~Shader();

	/**
	 *  @brief  Tries to compile Shader from sprcified source
	 *
	 *  Tries to comile shader and sets flag of success
	 *  @param  src Contain source code of the shader
	 *  @see [isCompiled]
	 */
	void compile(const std::string &src);

	/**
	 *  @brief  Returns internal error message
	 *
	 *  Returns compilation error if any.
	 *  @return String with error information. If no errors had occurred than returns empty string.
	 */
	std::string getErrorMessage() const { return m_error; }

	bool isCompiled() const { return m_compiled; }
private:
	friend class ShaderProgram;
	friend class ShaderLoader;

	GLint 	    m_compiled;
	std::string m_error;
}; 

}

#endif
