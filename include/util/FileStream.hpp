#ifndef ETO_FILE_STREAM
#define ETO_FILE_STREAM

#include <string>
#include <fstream>

namespace eto
{
/*!
 *  \brief  Represents file as a stream
 *
 *  FileStream is used for abstract file I/O.
 */
class FileStream
{
public:
	enum AccessMode 
	{
		Read = 1,
		Write
	};

	FileStream(const std::string &path, unsigned mode = AccessMode::Read);
	~FileStream();
			
	std::size_t read(void *buffer, std::size_t count);

	std::size_t write(const void *buffer, std::size_t count);

	std::size_t get_size() const { return m_size; } 

	void close();

	bool is_readable() const { return (m_mode & AccessMode::Read); }
	bool is_writable() const { return (m_mode & AccessMode::Write); }
	bool is_open() const { return m_open; }
private:
	std::fstream m_fs;
	std::size_t  m_size;
	unsigned     m_mode;
	bool 	     m_open;

}; // end of class FileStream  

}

#endif // ETO_FILE_STREAM
