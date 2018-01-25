#include <internal/Assert.hpp>
#include <iostream> // cerr, endl, flush
#include <cstdlib> // exit()
#include <mutex>

namespace assert_mutex { std::mutex eto_assert_mutex; }

void eto_Assert_error_handler(const char *message, const char *file_name, unsigned line)
{
	// This is a good place to put debug point and/or write into a log file
	assert_mutex::eto_assert_mutex.lock();
	std::cerr << "Error: " << message << " in file " << file_name << ":" << line << std::endl << std::flush;
	assert_mutex::eto_assert_mutex.unlock();
	exit(1);
}
