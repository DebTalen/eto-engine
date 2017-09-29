#include <catch/catch.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <resources/ModelLoader.hpp>

using namespace eto;

TEST_CASE("Mesh is loaded", "[Model]")
{
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	GLFWwindow *w = glfwCreateWindow(128, 128, "", NULL, NULL);
	glfwMakeContextCurrent(w);
	if (! gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))  
		FAIL( "Failed to load GLAD");

	std::vector<Vertex> v = {
		{ -0.5,  0.5, 0.0,},  // Top Left 
		{ 0.5,  0.5, 0.0,},  // Top Right
		{ -0.5, -0.5, 0.0,},  // Bottom Left
		{ 0.5, -0.5, 0.0}   // Bottom Right
	};
	std::vector<uint> i= {
		0, 3, 2,
		0, 1, 3
	};
	Mesh mesh(v, i);
	REQUIRE( glGetError() == GL_NO_ERROR );
	REQUIRE( mesh.numIndices == i.size() );
	REQUIRE( mesh.numVertices == v.size() );
}

TEST_CASE("Model is loaded", "[Model]")
{

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	GLFWwindow *w = glfwCreateWindow(128, 128, "", NULL, NULL);
	glfwMakeContextCurrent(w);
	if (! gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))  
		FAIL( "Failed to load GLAD");
	AssetLoader loader;
	SPtr<Model> model = std::make_shared<Model>();
	REQUIRE( model->isLoaded() == false );
	SECTION("Valid model")
	{
		model = loader.load<ModelLoader>("/home/morgoth/cpp/eto/tests/assets/testsphere.nff");
		REQUIRE( glGetError() == GL_NO_ERROR );
		REQUIRE( model->isLoaded() );
	}
	SECTION("Invalid model")
	{
		model = loader.load<ModelLoader>("non-existing");
		REQUIRE( model->isLoaded() == false );
		//WARN( model->getErrorMessage() );
	}
}

