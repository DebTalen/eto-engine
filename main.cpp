#include <iostream>
#include <cstdio>
#include <GLFW/glfw3.h>
#include <core/Window.hpp>
#include "util/FileStream.hpp"

void error_callback(int error, const char *description)
{
	printf("%d %sn", error, description);
}

using namespace eto;

#define STRINGFY2(X) #X
#define STRINGFY(X) STRINGFY2(X)
#define ROOT /home/morgoth/cpp/eto/


int main(void)
{
	FileStream fs("~/cpp/eto/assets/tests/fstreamtests.txt", FileStream::Write | FileStream::Read) ;
	fs.close();

	std::fstream f(STRINGFY(ROOT)"assets/tests/fstreamtests.txt", std::ios::binary | std::ios::in | std::ios::out);
	if (f.fail())
		std::cout << f.bad() << std::endl;

	std::cout << STRINGFY(ROOT) "assets/tests/fstreamtests.txt" << std::endl;
	return 0;
}

#undef ROOT
