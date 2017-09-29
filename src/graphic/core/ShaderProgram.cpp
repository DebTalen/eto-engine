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

