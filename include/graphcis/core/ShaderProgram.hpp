#ifndef ETO_SHADER_PROGRAM_HPP
#define ETO_SHADER_PROGRAM_HPP

#include <resources/Shader.hpp>
#include <glm/glm.hpp>

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
	void attach_shader(const Shader &sd) { glAttachShader(m_program, sd.m_handle.id); }
	void detach_shader(const Shader &sd) { glDetachShader(m_program, sd.m_handle.id); }
	void link();
	void use();

	bool is_linked() const { return m_linked; }

	std::string get_error_message() const { return m_error; }

	//GLint getUniformLocation(const std::string &name) { return glGetUniformLocation(m_program, name.c_str()); }

	/** Temporary function */
	GLuint get_raw() const { return m_program; }

	void set_int(const std::string &name, float val);
	void set_float(const std::string &name, float val);
	void set_vec2f(const std::string &name, glm::fvec2 vec);
	void set_vec2f(const std::string &name, float x, float y);
	void set_vec3f(const std::string &name, glm::fvec3 vec);
	void set_vec3f(const std::string &name, float x, float y, float z);
	void set_mat4f(const std::string &name, glm::fmat4 mat);

private:
	GLuint      m_program;
	GLint 	    m_linked;
	std::string m_error;
}; // end of class ShaderProgram

}

#endif
