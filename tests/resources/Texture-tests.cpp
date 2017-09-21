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
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	GLFWwindow *w = glfwCreateWindow(128, 128, "", NULL, NULL);
	glfwMakeContextCurrent(w);
	if (! gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))  
	{
		AssetLoader loader;
		SPtr<Texture> t = loader.load<TextureLoader>(STRINGFY(ROOT)"tests/assets/r64g64b64a255.png");
		REQUIRE( t != nullptr );
		REQUIRE( t->getDataSize() == (t->getWidth() * t->getHeight() * 4) );
	}
	else
		INFO("Failed to load GLAD");

}

TEST_CASE("Texture i/o", "[Texture]")
{
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	GLFWwindow *w = glfwCreateWindow(128, 128, "", NULL, NULL);
	glfwMakeContextCurrent(w);
	if (gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		AssetLoader loader;
		SECTION("Reading jpg")
		{
			SPtr<Texture> t = loader.load<TextureLoader>("tests/assets/r192g128b64.jpg");
			REQUIRE( t != nullptr );

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
		SECTION("Writting texture")
		{
			SPtr<Texture> t = loader.load<TextureLoader>("tests/assets/r64g64b64a255.png");
			REQUIRE( t != nullptr );

			std::vector<uchar> in(t->getDataSize());
			std::for_each(in.begin(), in.end(), [](uchar &var) { var = rand() % 255; } );

			REQUIRE( t->write(in) );
			std::vector<uchar> ot(t->getDataSize());
			t->read(ot);
			REQUIRE( ot == in );
		}
	}
	else 
		INFO("Failed to load GLAD");
}

#undef STRINGFY2
#undef STRINGFY
#undef ROOT
