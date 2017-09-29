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
			GLchar log[logSize];
			glGetProgramInfoLog(m_program, logSize, NULL, log);
			m_error = log;
		}
		else 
			m_error = "Unknown linking error";
	}
}

void ShaderProgram::use() 
{
	glUseProgram(m_program);
}


void ShaderProgram::setFloat(const std::string &name, float val)
{
	glUniform1f(glGetUniformLocation(m_program, name.c_str()), val);
}

void ShaderProgram::setVec2f(const std::string &name, glm::fvec2 vec)
{
	glUniform2f(glGetUniformLocation(m_program, name.c_str()), vec.x, vec.y);
}

void ShaderProgram::setVec3f(const std::string &name, glm::fvec3 vec)
{
	glUniform3f(glGetUniformLocation(m_program, name.c_str()), vec.x, vec.y, vec.z);
}

void ShaderProgram::setMat4f(const std::string &name, glm::fmat4 mat)
{
	glUniformMatrix4fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
