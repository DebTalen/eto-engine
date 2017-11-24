#include <resources/Shader.hpp>

using namespace eto;

Shader::Shader(ShaderType type)
	: m_compiled(GL_FALSE), m_error("")
{
	m_handle.id = glCreateShader(type);
}	

Shader::~Shader()
{
	glDeleteShader(m_handle.id);
}

void Shader::compile(const std::string &src)
{
	const GLchar *pSrc = src.c_str();
	glShaderSource(m_handle.id, 1, &pSrc, NULL);
	glCompileShader(m_handle.id);

	glGetShaderiv(m_handle.id, GL_COMPILE_STATUS, &m_compiled);
	if (m_compiled == GL_FALSE)
	{
		GLint logSize = 0;
		glGetShaderiv(m_handle.id, GL_INFO_LOG_LENGTH, &logSize);
		GLchar *log = new GLchar[logSize];
		glGetShaderInfoLog(m_handle.id, logSize, NULL, log);
		m_error = log;
		delete[] log;
	}
}
