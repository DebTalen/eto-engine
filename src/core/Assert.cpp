#include <core/Assert.hpp>
#include <iostream> // cerr, endl, flush
#include <stdlib.h> // exit()

void eto_AssertErrorHandler(const char *message, const char *fileName, unsigned line)
{
	// This is a good place to put debug point and/or write into a log file
	std::cerr << "Error: " << message << " in file " << fileName << ":" << line << std::endl << std::flush;
	exit(1);
}
