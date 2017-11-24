#include <catch/catch.hpp>

#include <resources/Shader.hpp>

#include <string>

using namespace eto;

TEST_CASE("Shader is loaded", "[Shader]")
{
	SECTION("Valid vertex shader")
	{
		std::string src("#version 330 core \n"
			       	"layout (location = 0) in vec3 aPos;\n"
				"out vec4 vColor; \n"
				"void main() { \n"
					"gl_Position = vec4(aPos, 1.0); \n"
					"vColor = vec4(0.5, 0, 0, 1); }");

		Shader s(VertexShader);
		s.compile(src);
		CHECK ( s.is_compiled() );
		if (! s.is_compiled() )
			FAIL( s.get_error_message() );
	}
	SECTION("Invalid vertex shader")
	{
		std::string src("");

		Shader s(VertexShader);
		s.compile(src);
		REQUIRE_FALSE( s.is_compiled() );
	}
	SECTION("Valid fragment shader")
	{
		std::string src("#version 330 core\n"
				"out vec4 color;\n"
				"void main() { color = vec4(1, 1, 1, 1); }");

		Shader s(FragmentShader);
		s.compile(src);
		CHECK ( s.is_compiled() );
		if (! s.is_compiled() )
			FAIL( s.get_error_message() );
	}
	SECTION("Invalid fragment shader")
	{
		std::string src("");

		Shader s(FragmentShader);
		s.compile(src);
		REQUIRE_FALSE( s.is_compiled() );
	}
	SECTION("Valid geometry shader")
	{
		std::string src("#version 330\n"
				"void main() { gl_Position = vec4(1, 1, 1, 1); }");

		Shader s(GeometryShader);
		s.compile(src);
		CHECK ( s.is_compiled() );
		if (! s.is_compiled() )
			INFO( s.get_error_message() );
	}
	SECTION("Invalid geometry shader")
	{
		std::string src("");

		Shader s(GeometryShader);
		s.compile(src);
		REQUIRE_FALSE( s.is_compiled() );
	}
}

