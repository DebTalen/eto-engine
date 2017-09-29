#ifndef ETO_SHADER_PROGRAM_HPP
#define ETO_SHADER_PROGRAM_HPP

#include <resources/Shader.hpp>

namespace eto
{


/**
 * @brief Just wraps OpenGL shader program
 */
class ShaderProgram
{
public:
	ShaderProgram ();
	~ShaderProgram ();
	void attachShader(const Shader &sd) { glAttachShader(m_program, sd.m_handle.id); }
	void detachShader(const Shader &sd) { glDetachShader(m_program, sd.m_handle.id); }
	void link();
	void use();

	bool isLinked() const { return m_linked; }

	std::string getErrorMessage() const { return m_error; }

	GLint getUniformLocation(const std::string &name) { return glGetUniformLocation(m_program, name.c_str()); }

	GLuint getRaw() const { return m_program; }
private:
	GLuint      m_program;
	GLint 	    m_linked;
	std::string m_error;
}; // end of class ShaderProgram
}

#endif
