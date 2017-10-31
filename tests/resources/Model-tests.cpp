#include <catch/catch.hpp>

#include <resources/ModelLoader.hpp>

using namespace eto;

TEST_CASE("Mesh is loaded", "[Model]")
{
	std::vector<Vertex> v = {
		{{ -0.5,  0.5, 0.0,}, {0.0f, 0.0f, -1.0f }, {0, 0}},// Top Left 
		{{ 0.5,  0.5, 0.0,},  {0.0f, 0.0f, -1.0f }, {0, 0}},// Top Right
		{{ -0.5, -0.5, 0.0,}, {0.0f, 0.0f, -1.0f }, {0, 0}},// Bottom Left
		{{ 0.5, -0.5, 0.0},   {0.0f, 0.0f, -1.0f }, {0, 0}} // Bottom Right
	};
	std::vector<uint> i= {
		0, 3, 2,
		0, 1, 3
	};
	Mesh mesh;
	mesh.setGeometry(v, i);
	REQUIRE( glGetError() == GL_NO_ERROR );
	REQUIRE( mesh.getNumIndices() == i.size() );
	REQUIRE( mesh.getNumVertices() == v.size() );

	Material material;
	material.textures.push_back(std::pair<Material::TextureType, std::shared_ptr<Texture>>(Material::TexDiffuse, nullptr));
	material.textures.push_back(std::pair<Material::TextureType, std::shared_ptr<Texture>>(Material::TexDiffuse, nullptr));
	mesh.setMaterial(material);
	REQUIRE( mesh.getNumTextures() == 2 );
}

TEST_CASE("Model is loaded", "[Model]")
{
	std::shared_ptr<ShaderProgram> sp = std::make_shared<ShaderProgram>();
	Model model(sp);
	REQUIRE( model.isLoaded() == false );
	REQUIRE( model.getNumMeshes() == 0 );
	REQUIRE( model.getMesh(0) == nullptr );
	REQUIRE( model.removeMesh(1) == false);
	REQUIRE( model.getTransform() == glm::mat4(1.0f) );
	REQUIRE( model.getShaderProgram() == sp );
	REQUIRE( model.getErrorMessage() == "");

	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
	model.addMesh(mesh);
	REQUIRE( model.isLoaded() == true );
	REQUIRE( model.getNumMeshes() == 1 );
	REQUIRE( model.getMesh(0) == mesh );
	std::shared_ptr<Mesh> mesh2 = std::make_shared<Mesh>();
	model.addMesh(mesh2);
	REQUIRE( model.isLoaded() == true );
	REQUIRE( model.getNumMeshes() == 2 );
	REQUIRE( model.getMesh(1) == mesh2 );
	REQUIRE( model.removeMesh(0) == true);
	REQUIRE( model.getNumMeshes() == 1 );
	REQUIRE( model.getMesh(0) == mesh2 );

	glm::mat4 mat = glm::mat4(20.0f);
	model.setTranform(mat);
	REQUIRE( model.getTransform() == mat );
}


