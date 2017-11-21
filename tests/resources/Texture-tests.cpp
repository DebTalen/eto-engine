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
		AssetLoader loader = AssetLoader::get_instance();
		std::shared_ptr<Texture> t = loader.load<TextureLoader>(STRINGFY(ROOT)"tests/assets/r64g64b64a255.png");
		if( t->is_loaded() != true )
			FAIL( t->get_error_message() );
		REQUIRE( t->get_data_size() == (t->get_width() * t->get_height() * t->get_components() * t->get_depth()) );
	}
	SECTION("via Constructor")
	{
		Texture t;
		REQUIRE( t.is_loaded() == false );
		REQUIRE( t.get_width() == 1 );
		REQUIRE( t.get_height() == 1 );
		REQUIRE( t.get_depth() == 1 );
		REQUIRE( t.get_data_size() == 3 );
		REQUIRE( t.is_mipmaped() == true );

		Texture::TextureProps tp;
		tp.width = tp.height = 10;
		tp.is_mipmap = false;
		Texture t2(tp);
		REQUIRE( t2.is_loaded() == false );
		REQUIRE( t2.get_width() == 10 );
		REQUIRE( t2.get_height() == 10 );
		REQUIRE( t2.get_depth() == 1 );
		REQUIRE( t2.get_data_size() == (tp.width * tp.height * 3) );
		REQUIRE( t2.is_mipmaped() == false );

		std::vector<uchar> data(tp.width * tp.height * 3);
		t2.write(data);
		REQUIRE( t2.is_loaded() == true );
	}
}

TEST_CASE("Texture i/o", "[Texture]")
{
	AssetLoader loader = AssetLoader::get_instance();
	SECTION("Reading jpg")
	{
		std::shared_ptr<Texture> t = loader.load<TextureLoader>("tests/assets/r192g128b64.jpg");
		REQUIRE( t->is_loaded() == true );

		std::vector<uchar> data(t->get_data_size());
		t->read(data);
		REQUIRE( data.size() == t->get_data_size() );

		int r = 192, g = 128, b = 64;
		bool image_data_is_correct = true;
		for (uint i = 0; i < data.size(); i += t->get_components())
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
		std::shared_ptr<Texture> t = loader.load<TextureLoader>("tests/assets/r64g64b64a255.png");
		REQUIRE( t != nullptr );

		std::vector<uchar> data(t->get_data_size());
		t->read(data);
		REQUIRE( data.size() == t->get_data_size() );

		int r = 64, a = 255;
		bool image_data_is_correct = true;
		for (uint i = 0; i < data.size(); i += t->get_components())
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
		std::shared_ptr<Texture> t = loader.load<TextureLoader>("tests/assets/r64g64b64a255.png");
		REQUIRE( t != nullptr );

		std::vector<uchar> in(t->get_data_size());
		std::for_each(in.begin(), in.end(), [](uchar &var) { var = rand() % 255; } );

		t->write(in);
		REQUIRE( t->is_loaded() == true );

		std::vector<uchar> ot(t->get_data_size());
		t->read(ot);
		REQUIRE( ot == in );
	}
}

#undef STRINGFY2
#undef STRINGFY
#undef ROOT
