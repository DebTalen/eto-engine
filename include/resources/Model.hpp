#ifndef ETO_MODEL_HPP
#define ETO_MODEL_HPP

#include <glad/glad.h>
#include <resources/AssetLoader.hpp>
#include <resources/Resource.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace eto
{


/** Represents single vertex of a mesh */
struct Vertex 
{
	Vertex(const glm::vec3 &position) 
		: pos(position)
	{
	}
	Vertex(float x, float y, float z)
		: pos(x, y, z)
	{
	}
	glm::vec3 pos;
};


/**
 *  @brief  Single drawable instance of model
 *  
 *  Holds appropriate GPU data.
 */
struct Mesh
{
	Mesh(const std::vector<Vertex> &vertices, const std::vector<uint> &indices);

	~Mesh();

	GLuint vao;
	uint numVertices;
	uint numIndices;
private:
	GLuint m_vbo;
	GLuint m_ebo;
};


/**
 *  @brief  Represents model
 *
 *  The Model consists of meshes loaded in GPU memory.
 */
class Model : public Resource
{
public:
	Model ();
	
	void loadModel(const std::vector<SPtr<Mesh>> &meshes/* , const std::vector<Texture> textures */);

	bool isLoaded() const { return m_loaded; }

	std::string getErrorMessage() const { return m_error; }

	/**
	 *  @brief  Temporary function for debuging purposes
	 */
	void draw();
private:
	friend class ModelLoader;
	void setErrorMessage(const std::string &err) { m_error = err; }

	std::vector<SPtr<Mesh>> m_meshes;
	// std::vector<Texture> m_textures;
	GLint 		  m_loaded;
	std::string 	  m_error;
};

}
#endif
