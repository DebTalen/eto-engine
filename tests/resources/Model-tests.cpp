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
	mesh.set_geometry(v, i);
	REQUIRE( glGetError() == GL_NO_ERROR );
	REQUIRE( mesh.get_num_indices() == i.size() );
	REQUIRE( mesh.get_num_vertices() == v.size() );

	Material material;
	material.textures.push_back(std::pair<Material::TextureType, std::shared_ptr<Texture>>(Material::TexDiffuse, nullptr));
	material.textures.push_back(std::pair<Material::TextureType, std::shared_ptr<Texture>>(Material::TexDiffuse, nullptr));
	mesh.set_material(material);
	REQUIRE( mesh.get_num_textures() == 2 );
}

TEST_CASE("Model is loaded", "[Model]")
{
	std::shared_ptr<ShaderProgram> sp = std::make_shared<ShaderProgram>();
	Model model(sp);
	REQUIRE( model.is_loaded() == false );
	REQUIRE( model.get_num_meshes() == 0 );
	REQUIRE( model.get_mesh(0) == nullptr );
	REQUIRE( model.remove_mesh(1) == false);
	REQUIRE( model.get_transform() == glm::mat4(1.0f) );
	REQUIRE( model.get_shader_program() == sp );
	REQUIRE( model.get_error_message() == "");

	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
	model.add_mesh(mesh);
	REQUIRE( model.is_loaded() == true );
	REQUIRE( model.get_num_meshes() == 1 );
	REQUIRE( model.get_mesh(0) == mesh );
	std::shared_ptr<Mesh> mesh2 = std::make_shared<Mesh>();
	model.add_mesh(mesh2);
	REQUIRE( model.is_loaded() == true );
	REQUIRE( model.get_num_meshes() == 2 );
	REQUIRE( model.get_mesh(1) == mesh2 );
	REQUIRE( model.remove_mesh(0) == true);
	REQUIRE( model.get_num_meshes() == 1 );
	REQUIRE( model.get_mesh(0) == mesh2 );

	glm::mat4 mat = glm::mat4(20.0f);
	model.set_tranform(mat);
	REQUIRE( model.get_transform() == mat );
}


