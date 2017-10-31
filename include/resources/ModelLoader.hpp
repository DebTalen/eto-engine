#ifndef ETO_MODEL_LOADER_HPP
#define ETO_MODEL_LOADER_HPP

#include <resources/AssetLoader.hpp>
#include <resources/Model.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace eto
{

/**
 *  @brief Helper class for loading models from file
 *
 *  Loads model using Assimp library
 */
class ModelLoader 
{
public:
	/** Flags of additional steps after loading */
	enum Flags {
		FlagTriangulate = aiProcess_Triangulate,
		FlagGenNormals = aiProcess_GenNormals,
		FlagOptimize   = aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes
	};
	/**
	 *  @brief  Loads model sprcified by path
	 *
	 *  @param  path Path to the model file
	 *  @return Shared pointer to the loaded Model.
	 */
	static std::shared_ptr<Model> load(const std::string &path,
				const std::shared_ptr<ShaderProgram> pShader, 
				int loadingFlags = Flags::FlagTriangulate |
						   Flags::FlagGenNormals  |
						   Flags::FlagOptimize  ); 
private:
	ModelLoader () {}

	static void processNode(const aiScene *scene, const aiNode *node, std::shared_ptr<Model> model, const std::string &path);

	static std::shared_ptr<Mesh> processMesh(const std::string &path, const aiScene *scene, const aiMesh *mesh);

	static void processMaterial(const aiMaterial *mat, aiTextureType aiType, Material &material, Material::TextureType type, const std::string &path);

};

}

#endif 
