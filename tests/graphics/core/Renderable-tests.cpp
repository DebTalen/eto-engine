#include <catch/catch.hpp>
#include <resources/AssetLoader.hpp>
#include <graphcis/core/Renderable.hpp>

using namespace eto;

TEST_CASE("Renderable is created", "[Renderable]")
{
	SPtr<Model> model;
	Renderable r(model);
	glm::vec3 null_vec(0.0f);
	glm::mat4 mat = glm::mat4(1.0f);

	REQUIRE( r.getTransform() == mat );
}

TEST_CASE("Renderable is transformed", "[Renderable]")
{
	SPtr<Model> model;
	Renderable r(model);
	glm::mat4 mat = glm::mat4(1.0f);


	mat = glm::translate(mat, glm::vec3(0, 1.0f, 0));
	r.translate(glm::vec3(0, 1.0f, 0));
	REQUIRE( r.getTransform() == mat );

	mat = glm::translate(mat, glm::vec3(-100, 1.0f, 1203124));
	r.translate(glm::vec3(-100, 1.0f, 1203124));
	REQUIRE( r.getTransform() == mat );

	mat = glm::rotate(mat, glm::radians(1.0f), glm::vec3(1));
	r.rotate(1, glm::vec3(1));
	REQUIRE( r.getTransform() == mat );

	mat = glm::rotate(mat, glm::radians(-998.0f), glm::vec3(2, 0, 0));
	r.rotate(-998, glm::vec3(2, 0, 0));
	REQUIRE( r.getTransform() == mat );

	mat = glm::scale(mat, glm::vec3(2, 0, 0));
	r.scale(glm::vec3(2, 0, 0));
	REQUIRE( r.getTransform() == mat );

	mat = glm::scale(mat, glm::vec3(2, 900124, -11));
	r.scale(glm::vec3(2, 900124, -11));
	REQUIRE( r.getTransform() == mat );

}
