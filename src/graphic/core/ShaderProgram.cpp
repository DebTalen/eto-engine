#include <graphcis/core/ShaderProgram.hpp>

using namespace eto;

ShaderProgram::ShaderProgram() 
	: m_linked(0), m_error("")
{
	m_program = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_program);
}

void ShaderProgram::link()
{
	glLinkProgram(m_program);
	glGetProgramiv(m_program, GL_LINK_STATUS, &m_linked);
	if (m_linked == GL_FALSE)
	{
		GLint logSize = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logSize);
		if (logSize)
		{
			GLchar *log = new GLchar[logSize];
			glGetProgramInfoLog(m_program, logSize, NULL, log);
			m_error = log;
			delete[] log;
		}
		else 
			m_error = "Unknown linking error";
	}
}

void ShaderProgram::use() 
{
	glUseProgram(m_program);
}

void ShaderProgram::set_int(const std::string &name, float val)
{
	glUniform1i(glGetUniformLocation(m_program, name.c_str()), val);
}

void ShaderProgram::set_float(const std::string &name, float val)
{
	glUniform1f(glGetUniformLocation(m_program, name.c_str()), val);
}

void ShaderProgram::set_vec2f(const std::string &name, glm::fvec2 vec)
{
	glUniform2f(glGetUniformLocation(m_program, name.c_str()), vec.x, vec.y);
}

void ShaderProgram::set_vec2f(const std::string &name, float x, float y)
{
	glUniform2f(glGetUniformLocation(m_program, name.c_str()), x, y);
}

void ShaderProgram::set_vec3f(const std::string &name, glm::fvec3 vec)
{
	glUniform3f(glGetUniformLocation(m_program, name.c_str()), vec.x, vec.y, vec.z);
}

void ShaderProgram::set_vec3f(const std::string &name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(m_program, name.c_str()), x, y, z);
}

void ShaderProgram::set_mat4f(const std::string &name, glm::fmat4 mat)
{
	glUniformMatrix4fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
