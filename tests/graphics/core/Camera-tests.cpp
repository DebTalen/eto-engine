#include <catch/catch.hpp>
#include <graphcis/core/Camera.hpp>

using namespace eto;
using glm::vec3;

TEST_CASE("Camera is created", "[Camera]")
{
	Camera cam(vec2(100, 100), vec3(0, 0, -3.0f));
	REQUIRE( cam.get_pos() == vec3(0, 0, -3.0f) );
	REQUIRE( cam.get_rotate() == vec3(0.0f) );
}


TEST_CASE("Camrea movements", "[Camera]")
{
	Camera cam(vec2(100, 100), vec3(0, 0, -3.0f));
	cam.set_pos(vec3(10, 10, 10));
	REQUIRE( cam.get_pos() == vec3(10, 10, 10) );
	cam.rotate(vec3(45.0f, 45.0f, 40.0f));
	REQUIRE( cam.get_rotate() == vec3(45.0f, 45.0f, 40.0f) );
	
	cam.move({10, 10, 10});
	REQUIRE( cam.get_pos() == vec3(20, 20, 20) );
	cam.rotate(vec3(0, 0, -15.0f));
	REQUIRE( cam.get_rotate() == vec3(45.0f, 45.0f, 25.0f) );
	// get ViewMatrix
}
