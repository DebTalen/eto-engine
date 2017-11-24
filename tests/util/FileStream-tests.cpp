#include <catch/catch.hpp>
#include <util/FileStream.hpp>
#include <cstdio>

using namespace eto;
#define STRINGFY2(X) #X
#define STRINGFY(X) STRINGFY2(X)
#define ROOT /home/morgoth/cpp/eto/

TEST_CASE("FileStream is created", "[FileStream]")
{
	SECTION("Existing file read only mode") 
	{
		FileStream fs(STRINGFY(ROOT) "tests/assets/fstreamtests.txt");
		REQUIRE( fs.is_open() );
		REQUIRE( fs.is_readable() );
		REQUIRE( fs.is_writable() == 0 );
		fs.close();
	}
	SECTION("Non-existing file read only mode")
	{
		FileStream fs(STRINGFY(ROOT) "/non-existing");
		REQUIRE( fs.is_open() == 0 );
	}
	SECTION("Existing file read and write mode")
	{
		FileStream fs(STRINGFY(ROOT) "tests/assets/fstreamtests.txt", FileStream::Write | FileStream::Read) ;
		REQUIRE( fs.is_open() );
		REQUIRE( fs.is_readable() );
		REQUIRE( fs.is_writable() );
		fs.close();
	}
	SECTION("Non-existing file read and write mode")
	{
		FileStream fs(STRINGFY(ROOT) "/non-existing", FileStream::Write | FileStream::Read);
		REQUIRE( fs.is_open() == 0 );
	}
}

TEST_CASE("FileStream is outputted", "[FileStream]")
{
	char buf[128];
	for (int i = 0; i < 128; ++i)
		buf[i] = rand() % 50;
	::remove(STRINGFY(ROOT) "tests/assets/fstreamtests.txt");
	FileStream fs(STRINGFY(ROOT) "tests/assets/fstreamtests.txt", FileStream::Write);
	REQUIRE( fs.is_open() );
	SECTION("via write() function")
	{
		REQUIRE( fs.write(buf, 128) == 128 );
	}
}

TEST_CASE("FileStream is inputted", "[FileStream]")
{
	char buf[128];
	int b = 10;
	FileStream fs(STRINGFY(ROOT) "tests/assets/fstreamtests.txt");
	REQUIRE( fs.is_open() );
	SECTION("via read() function")
	{
		REQUIRE( fs.read(buf, 128) == 128 );
	}
}

#undef ROOT
#undef STRINGFY
#undef STRINGFY2
