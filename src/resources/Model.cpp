#include <resources/Model.hpp>

using namespace eto;

Mesh::Mesh()
	: m_numIndices(0), m_numVertices(0), m_loaded(0)
{
}
	
void Mesh::setGeometry(const vector<Vertex> &vertices, const vector<uint> &indices)
{
	if (m_loaded) 
		return;
	m_numIndices = indices.size();
	m_numVertices = vertices.size();

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
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
//	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	m_loaded = true;
}

void Mesh::setMaterial(const Material &material)
{
	m_material = material;
}

Mesh::~Mesh() 
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
	glDeleteVertexArrays(1, &m_vao);
}

Model::Model(const SPtr<ShaderProgram> pShader)
	: m_loaded(0), m_error(""), m_shader(pShader)
{
	ETO_ASSERT( m_shader != nullptr );
	m_modelTransform = glm::mat4(1.0f);
}

void Model::addMesh(const SPtr<Mesh> pMesh)
{
	m_meshes.push_back(pMesh); 
}

bool Model::removeMesh(uint index)
{
	if (index >= m_meshes.size()) 
		return false;
	m_meshes.erase(m_meshes.begin() + index);
	if (m_meshes.size() == 0)
		m_loaded = false;
	return true;
}

std::string Model::getErrorMessage() const 
{
	return m_error;
}


