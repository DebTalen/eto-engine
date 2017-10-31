#ifndef ETO_MODEL_HPP
#define ETO_MODEL_HPP

#include <glad/glad.h>
#include <vector>
#include <glm/vec3.hpp>
#include <iostream>

#include <core/Assert.hpp>
#include <resources/AssetLoader.hpp>
#include <resources/Resource.hpp>
#include <resources/Texture.hpp>
#include <graphcis/core/ShaderProgram.hpp>

using std::vector;
using std::pair;
using glm::vec3;
using glm::vec2;
typedef unsigned int uint;

namespace eto
{

/** Represents single vertex of a mesh */
struct Vertex 
{
	vec3 pos;
	vec3 normal;
	vec2 texCoord;
};

/** Represents materials of a mesh */
struct Material 
{
	enum TextureType 
	{
		TexDiffuse,
		TexSpecular
	};
	vector< pair<TextureType, std::shared_ptr<Texture>> > textures;
	vec3 color_ambient = vec3(1);
	vec3 color_diffuse = vec3(1);
	vec3 color_specular = vec3(1);
	float shinines = 32;
};

/**
 *  @brief  Single drawable instance 
 *  
 *  Holds appropriate GPU data.
 */
class Mesh : public Resource
{
public:
	// Should also set draw type, e.g. STATIC_DRAW
	Mesh ();
	~Mesh ();

	/**
	 *  @brief  Loads the specified geometry data to GPU
	 *
	 *  Indices are used to tell the GPU in which order to draw vertices
	 *  @param  vertices Set of mesh vertices
	 *  @oaram  indices  Set of mesh indices 
	 */
	void setGeometry(const vector<Vertex> &vertices,
			 const vector<uint>   &indices );
	/**
	 *  @brief  Sets the specidied material to the mesh
	 *
	 *  The Material is used for mesh rendering to achieve more realistic output
	 *  @param  material The Material to set
	 */
	void setMaterial(const Material &material);

	int getNumTextures() const { return m_material.textures.size(); }
	int getNumVertices() const { return m_numVertices; }
	int getNumIndices() const { return m_numIndices; } 
public:
	/** You cannot copy a Mesh because it has assosiated GPU data that will be freed after rhs destruction */
	Mesh (const Mesh &rhs) = delete;
	Mesh &operator= (const Mesh &rhs) = delete;

	// Temporary. Should be replaced by Renderer
	friend class Renderable;

	GLuint 	m_vao;
	GLuint 	m_vbo;
	GLuint	m_ebo;
	GLint   m_numIndices;
	GLint   m_numVertices;
	bool 	m_loaded;
	Material m_material;
//	std::shared_ptr<ShaderProgram> m_shader;
};

class Model : public Resource
{
public:
	// Temporary  single shader program per model
	// In future should load Model from xml where 
	// shaders will be specified for each mesh or not ...
	Model(const std::shared_ptr<ShaderProgram> pShader);

	void addMesh(const std::shared_ptr<Mesh> pMesh);

	bool removeMesh(uint index);

	uint getNumMeshes() const { return m_meshes.size(); }

	std::shared_ptr<Mesh> getMesh(uint index) const { return (index >= m_meshes.size()) ? nullptr : m_meshes[index]; }

	bool isLoaded() const { return m_meshes.size(); }

	std::shared_ptr<ShaderProgram> getShaderProgram() const { return m_shader; }

	std::string getErrorMessage() const;

	glm::mat4 getTransform() const { return m_modelTransform; }

	void setTranform(const glm::mat4 MtW) { m_modelTransform = MtW; }

public:
	friend class ModelLoader;
	void setErrorMessage(const std::string error) { m_error = error; }

	bool 		    m_loaded;
	std::string 	    m_error;
	glm::mat4 	    m_modelTransform;
	vector<std::shared_ptr<Mesh>>  m_meshes;
	std::shared_ptr<ShaderProgram> m_shader;
};

}
#endif
