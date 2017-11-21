#include <core/Assert.hpp>
#include <iostream> // cerr, endl, flush
#include <cstdlib> // exit()

void eto_Assert_error_handler(const char *message, const char *file_name, unsigned line)
{
	// This is a good place to put debug point and/or write into a log file
	std::cerr << "Error: " << message << " in file " << file_name << ":" << line << std::endl << std::flush;
	exit(1);
}
