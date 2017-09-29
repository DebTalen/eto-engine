#include <catch/catch.hpp>

#include <resources/Shader.hpp>
#include <GLFW/glfw3.h>

#include <string>

using namespace eto;

TEST_CASE("Shader is loaded", "[Shader]")
{
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	GLFWwindow *w = glfwCreateWindow(128, 128, "", NULL, NULL);
	glfwMakeContextCurrent(w);
	REQUIRE( gladLoadGLLoader((GLADloadproc) glfwGetProcAddress));

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
		CHECK ( s.isCompiled() );
		if (! s.isCompiled() )
			FAIL( s.getErrorMessage() );
	}
	SECTION("Invalid vertex shader")
	{
		std::string src("");

		Shader s(VertexShader);
		s.compile(src);
		REQUIRE_FALSE( s.isCompiled() );
	}
	SECTION("Valid fragment shader")
	{
		std::string src("#version 330 core\n"
				"out vec4 color;\n"
				"void main() { color = vec4(1, 1, 1, 1); }");

		Shader s(FragmentShader);
		s.compile(src);
		CHECK ( s.isCompiled() );
		if (! s.isCompiled() )
			FAIL( s.getErrorMessage() );
	}
	SECTION("Invalid fragment shader")
	{
		std::string src("");

		Shader s(FragmentShader);
		s.compile(src);
		REQUIRE_FALSE( s.isCompiled() );
	}
	SECTION("Valid geometry shader")
	{
		std::string src("#version 330\n"
				"void main() { gl_Position = vec4(1, 1, 1, 1); }");

		Shader s(GeometryShader);
		s.compile(src);
		CHECK ( s.isCompiled() );
		if (! s.isCompiled() )
			INFO( s.getErrorMessage() );
	}
	SECTION("Invalid geometry shader")
	{
		std::string src("");

		Shader s(GeometryShader);
		s.compile(src);
		REQUIRE_FALSE( s.isCompiled() );
	}
}

