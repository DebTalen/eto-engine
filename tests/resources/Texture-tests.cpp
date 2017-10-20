#include <catch/catch.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <resources/TextureLoader.hpp>
#include <resources/AssetLoader.hpp>

using namespace eto;

#define STRINGFY2(X) #X
#define STRINGFY(X) STRINGFY2(X)
#define ROOT /home/morgoth/cpp/eto/

TEST_CASE("Texture is created", "[Texture]")
{
	SECTION("via AssetLoader")
	{
		AssetLoader loader = AssetLoader::getInstance();
		SPtr<Texture> t = loader.load<TextureLoader>(STRINGFY(ROOT)"tests/assets/r64g64b64a255.png");
		if( t->isLoaded() != true )
			FAIL( t->getErrorMessage() );
		REQUIRE( t->getDataSize() == (t->getWidth() * t->getHeight() * t->getComponents() * t->getDepth()) );
	}
	SECTION("via Constructor")
	{
		Texture t;
		REQUIRE( t.isLoaded() == false );
		REQUIRE( t.getWidth() == 1 );
		REQUIRE( t.getHeight() == 1 );
		REQUIRE( t.getDepth() == 1 );
		REQUIRE( t.getDataSize() == 3 );
		REQUIRE( t.isMipmaped() == true );

		Texture::TextureProps tp;
		tp.width = tp.height = 10;
		tp.isMipmap = false;
		Texture t2(tp);
		REQUIRE( t2.isLoaded() == false );
		REQUIRE( t2.getWidth() == 10 );
		REQUIRE( t2.getHeight() == 10 );
		REQUIRE( t2.getDepth() == 1 );
		REQUIRE( t2.getDataSize() == (tp.width * tp.height * 3) );
		REQUIRE( t2.isMipmaped() == false );

		std::vector<uchar> data(tp.width * tp.height * 3);
		t2.write(data);
		REQUIRE( t2.isLoaded() == true );
	}
}

TEST_CASE("Texture i/o", "[Texture]")
{
	AssetLoader loader = AssetLoader::getInstance();
	SECTION("Reading jpg")
	{
		SPtr<Texture> t = loader.load<TextureLoader>("tests/assets/r192g128b64.jpg");
		REQUIRE( t->isLoaded() == true );

		std::vector<uchar> data(t->getDataSize());
		t->read(data);
		REQUIRE( data.size() == t->getDataSize() );

		int r = 192, g = 128, b = 64;
		bool image_data_is_correct = true;
		for (uint i = 0; i < data.size(); i += t->getComponents())
		{
			if (data[i] != r ||
			    data[i + 1] != g ||
			    data[i + 2] != b )
			{
				image_data_is_correct = false;
				break;
			}
		}
		REQUIRE( image_data_is_correct == true );
	} 
	 SECTION("Reading png")
	{
		SPtr<Texture> t = loader.load<TextureLoader>("tests/assets/r64g64b64a255.png");
		REQUIRE( t != nullptr );

		std::vector<uchar> data(t->getDataSize());
		t->read(data);
		REQUIRE( data.size() == t->getDataSize() );

		int r = 64, a = 255;
		bool image_data_is_correct = true;
		for (uint i = 0; i < data.size(); i += t->getComponents())
		{
			if (data[i] != r ||
			    data[i + 1] != r ||
			    data[i + 2] != r ||
			    data[i + 3] != a)
			{
				image_data_is_correct = false;
				break;
			}
		}
		REQUIRE( image_data_is_correct == true );
	}
	SECTION("Writting to the texture")
	{
		SPtr<Texture> t = loader.load<TextureLoader>("tests/assets/r64g64b64a255.png");
		REQUIRE( t != nullptr );

		std::vector<uchar> in(t->getDataSize());
		std::for_each(in.begin(), in.end(), [](uchar &var) { var = rand() % 255; } );

		t->write(in);
		REQUIRE( t->isLoaded() == true );

		std::vector<uchar> ot(t->getDataSize());
		t->read(ot);
		REQUIRE( ot == in );
	}
}

#undef STRINGFY2
#undef STRINGFY
#undef ROOT
