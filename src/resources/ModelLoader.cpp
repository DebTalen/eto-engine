#include <resources/ModelLoader.hpp>

using namespace eto;

SPtr<Model> ModelLoader::load(const std::string &path)
{
	SPtr<Model> pModel =  std::make_shared<Model>();

	Assimp::Importer imp;
	const aiScene *scene = imp.ReadFile(path, aiProcess_Triangulate | aiProcess_OptimizeMeshes );
	if (scene == NULL || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
		pModel->setErrorMessage(imp.GetErrorString());
		return pModel;
	}

	std::vector<SPtr<Mesh>> meshes;
	processNode(scene, scene->mRootNode, meshes);
	pModel->loadModel(meshes);
	return pModel;
}

void ModelLoader::processNode(const aiScene *scene, const aiNode *node, std::vector<SPtr<Mesh>> &m/*, std::vector<Texture> t */)
{
	std::vector<Vertex> vertices;
	std::vector<uint> indices;
	for (uint i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(scene, mesh, vertices, indices);
		m.push_back(std::make_shared<Mesh>(vertices, indices));
		vertices.clear();
		indices.clear();
	}
	for (uint i = 0; i < node->mNumChildren; ++i)
	{
		processNode(scene, node->mChildren[i], m);
	}
}

void ModelLoader::processMesh(const aiScene *scene, const aiMesh *mesh, std::vector<Vertex> &v, std::vector<uint> &ic)
{
	v.reserve(mesh->mNumVertices);
	for (uint i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vr ( mesh->mVertices[i].x, mesh->mVertices[i].y,	mesh->mVertices[i].z);
		v.push_back(vr);
	}

	for(unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		const aiFace *face = &mesh->mFaces[i];
		for(unsigned int j = 0; j < face->mNumIndices; ++j)
			ic.push_back(face->mIndices[j]);
	}  
}
