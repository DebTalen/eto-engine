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
	/**
	 *  @brief  Loads model sprcified by path
	 *
	 *  @param  path Path to the model file
	 *  @return Shared pointer to the loaded Model.
	 */
	static SPtr<Model> load(const std::string &path);
private:
	ModelLoader () {}

	static void processNode(const aiScene *scene, const aiNode *node, std::vector<SPtr<Mesh>> &m/*, std::vector<Texture> t */);

	static void processMesh(const aiScene *scene, const aiMesh *mesh, std::vector<Vertex> &v, std::vector<uint> &ic);
};

}

#endif 
