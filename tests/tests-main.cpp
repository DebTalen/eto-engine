#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <core/Assert.hpp>

int main( int argc, char *argv[] )
{
	glfwInit();
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	GLFWwindow *w = glfwCreateWindow(128, 128, "", NULL, NULL);
	glfwMakeContextCurrent(w);
	if (! gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))  
		FAIL( "Failed to load GLAD");

	Catch::Session session;
	int result = session.run( argc, argv );

	glfwDestroyWindow(w);
	glfwTerminate();
	return ( result < 0xFF ? result : 0xFF );
}
