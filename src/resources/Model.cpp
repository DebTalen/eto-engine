#include <resources/Model.hpp>

using namespace eto;

Mesh::Mesh()
	: m_num_indices(0), m_num_vertices(0), m_loaded(0)
{
}
	
void Mesh::set_geometry(const vector<Vertex> &vertices, const vector<uint> &indices)
{
	if (m_loaded) 
		return;
	m_num_indices = indices.size();
	m_num_vertices = vertices.size();

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);

	// vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
//	glEnableVertexAttribArray(0);
	// vertex normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
//	glEnableVertexAttribArray(1);
	// texture coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
//	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	m_loaded = true;
}

void Mesh::set_material(const Material &material)
{
	m_material = material;
}

Mesh::~Mesh() 
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
	glDeleteVertexArrays(1, &m_vao);
}

Model::Model(const std::shared_ptr<ShaderProgram> pShader)
	: m_loaded(0), m_error(""), m_shader(pShader)
{
	ETO_ASSERT( m_shader != nullptr );
	m_model_transform = glm::mat4(1.0f);
}

void Model::add_mesh(const std::shared_ptr<Mesh> pMesh)
{
	m_meshes.push_back(pMesh); 
}

bool Model::remove_mesh(uint index)
{
	if (index >= m_meshes.size()) 
		return false;
	m_meshes.erase(m_meshes.begin() + index);
	if (m_meshes.size() == 0)
		m_loaded = false;
	return true;
}

std::string Model::get_error_message() const 
{
	return m_error;
}


