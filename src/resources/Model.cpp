#include <resources/Model.hpp>
#include <iostream>

using namespace eto;

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<uint> &indices)
{
	numIndices = indices.size();
	numVertices = vertices.size();

	/* for (uint i = 0; i < numVertices; ++i)
		std::cout << vertices[i].pos.x << " " <<vertices[i].pos.y << " " << vertices[i].pos.z << std::endl;
	std::cout << std::endl;

	for (uint i = 0; i < numIndices; ++i)
		std::cout << indices[i] << " ";
	std::cout << std::endl;
 */
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);

	// vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

Mesh::~Mesh() 
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
	glDeleteVertexArrays(1, &vao);
}

Model::Model()
	: m_loaded(0), m_error("")
{
}

void Model::loadModel(const std::vector<SPtr<Mesh>> &meshes)
{
	m_meshes.reserve(meshes.size());
	m_meshes = meshes;
	m_loaded = 1;
}

void Model::draw()
{
	if (m_loaded) 
	{
		for (uint i = 0; i < m_meshes.size(); ++i)
		{
			glBindVertexArray(m_meshes[i]->vao);
			glDrawElements(GL_TRIANGLES, m_meshes[i]->numIndices, GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0);
		glDisableVertexAttribArray(0);
	}
}
