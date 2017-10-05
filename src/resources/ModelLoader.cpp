#include <resources/ModelLoader.hpp>
#include <resources/AssetLoader.hpp>
#include <resources/TextureLoader.hpp>

using namespace eto;

SPtr<Model> ModelLoader::load(const std::string &path, const SPtr<ShaderProgram> pShader, int loadingFlags)
{
	SPtr<Model> root =  std::make_shared<Model>(pShader);

	Assimp::Importer imp;
	const aiScene *scene = imp.ReadFile(path, loadingFlags);

	if (scene == NULL || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
		root->setErrorMessage(imp.GetErrorString());
		return root;
	}

	processNode(scene, scene->mRootNode, root, path);
	return root;
}

void ModelLoader::processNode(const aiScene *scene, const aiNode *node, SPtr<Model> parent, const std::string &path)
{
	for (uint i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh *m = scene->mMeshes[node->mMeshes[i]];
		parent->addMesh(processMesh(path, scene, m));
	}
	for (uint i = 0; i < node->mNumChildren; ++i)
		processNode(scene, node->mChildren[i], parent,  path);
}

SPtr<Mesh> ModelLoader::processMesh(const std::string &path, const aiScene *scene, const aiMesh *m)
{
	std::vector<Vertex> v;
	std::vector<uint> ic;
	for (uint i = 0; i < m->mNumVertices; ++i)
	{
		Vertex vr;
		vr.pos = {m->mVertices[i].x, m->mVertices[i].y, m->mVertices[i].z};
		vr.normal = {m->mNormals[i].x,  m->mNormals[i].y, m->mNormals[i].z};

		if (m->mTextureCoords[0])
			vr.texCoord = { m->mTextureCoords[0][i].x, m->mTextureCoords[0][i].y };
		else
			vr.texCoord = {0, 0};
		if (m->mTextureCoords[1])
			std::cout << "HELLO THERE ARE MORE TEXTURE COORDINATES\n";

		v.push_back(vr);
	}

	for(unsigned int i = 0; i < m->mNumFaces; ++i)
	{
		const aiFace *face = &m->mFaces[i];
		for(unsigned int j = 0; j < face->mNumIndices; ++j)
			ic.push_back(face->mIndices[j]);
	}  

	SPtr<Mesh> res = std::make_shared<Mesh>();
	res->setGeometry(v, ic);

	std::string dir = path.substr(0, path.find_last_of('/'));
	Material material;
	aiMaterial *mat = scene->mMaterials[m->mMaterialIndex];

	processMaterial(mat, aiTextureType_DIFFUSE, material, Material::TexDiffuse, dir);
	processMaterial(mat, aiTextureType_SPECULAR, material, Material::TexSpecular, dir);

	res->setMaterial(material);
	return res;
}

void ModelLoader::processMaterial(const aiMaterial *mat, aiTextureType aiType, Material &material, Material::TextureType type, const std::string &path)
{
	AssetLoader loader = AssetLoader::getInstance();
	for (uint i = 0; i < mat->GetTextureCount(aiType); ++i)
	{
		aiString relPath;
		mat->GetTexture(aiType, i, &relPath);

		std::string str(relPath.C_Str());
		std::replace(str.begin(), str.end(), '\\', '/');
		if (str.at(0) != '/')
			str.insert(0, "/");
		std::cout << str << std::endl;

		SPtr<Texture> texture = loader.load<TextureLoader>(std::string(path + str));
		// temporary measure 
		if (texture == nullptr)
			continue;
		material.textures.push_back(pair<Material::TextureType, SPtr<Texture>>(type, texture));
	}
}
