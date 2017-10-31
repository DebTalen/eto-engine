#include <util/FileStream.hpp>

using std::size_t;

using namespace eto;

FileStream::FileStream(const std::string &path, unsigned accessMode)
	: m_mode(accessMode)
{
	std::ios::openmode mode = std::ios::binary;
	if (m_mode & Read)
		mode |= std::ios::in;
	if (m_mode & Write)
		mode |= std::ios::out;
	m_fs.open(path, mode);

	m_open = !m_fs.fail();

	if (m_open)
	{
		m_fs.seekg(0, std::ios_base::end);
		m_size = m_fs.tellg();
		m_fs.seekg(0, std::ios_base::beg);
	}
	else 
		m_open = 0;
}

FileStream::~FileStream()
{
	close();
}

size_t FileStream::read(void *buffer, size_t count)
{
	if (m_open && isReadable())
	{
		m_fs.read(static_cast<char*>(buffer), static_cast<std::streamsize>(count));
		return static_cast<size_t>(m_fs.gcount());
	}
	return 0;
}

size_t FileStream::write(const void *buffer, size_t count)
{
	if (m_open && isWritable())
	{
		m_fs.write(static_cast<const char*>(buffer), static_cast<std::streamsize>(count));
		return count;
	}
	return 0;
}

void FileStream::close() 
{
	if (m_open)
	{
		m_fs.flush();
		m_fs.close();
	}
}
