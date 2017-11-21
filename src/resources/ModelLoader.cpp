#include <resources/ModelLoader.hpp>
#include <resources/AssetLoader.hpp>
#include <resources/TextureLoader.hpp>

using namespace eto;

std::shared_ptr<Model> ModelLoader::load(const std::string &path, const std::shared_ptr<ShaderProgram> pShader, int loading_flags)
{
	std::shared_ptr<Model> root =  std::make_shared<Model>(pShader);

	Assimp::Importer imp;
	const aiScene *scene = imp.ReadFile(path, loading_flags);

	if (scene == NULL || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
		root->set_error_message(imp.GetErrorString());
		return root;
	}

	process_node(scene, scene->mRootNode, root, path);
	return root;
}

void ModelLoader::process_node(const aiScene *scene, const aiNode *node, std::shared_ptr<Model> parent, const std::string &path)
{
	for (uint i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh *m = scene->mMeshes[node->mMeshes[i]];
		parent->add_mesh(process_mesh(path, scene, m));
	}
	for (uint i = 0; i < node->mNumChildren; ++i)
		process_node(scene, node->mChildren[i], parent,  path);
}

std::shared_ptr<Mesh> ModelLoader::process_mesh(const std::string &path, const aiScene *scene, const aiMesh *m)
{
	std::vector<Vertex> v;
	std::vector<uint> ic;
	for (uint i = 0; i < m->mNumVertices; ++i)
	{
		Vertex vr;
		vr.pos = {m->mVertices[i].x, m->mVertices[i].y, m->mVertices[i].z};
		vr.normal = {m->mNormals[i].x,  m->mNormals[i].y, m->mNormals[i].z};

		if (m->mTextureCoords[0])
			vr.tex_coord = { m->mTextureCoords[0][i].x, m->mTextureCoords[0][i].y };
		else
			vr.tex_coord = {0, 0};
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

	std::shared_ptr<Mesh> res = std::make_shared<Mesh>();
	res->set_geometry(v, ic);

	std::string dir = path.substr(0, path.find_last_of('/'));
	Material material;
	aiMaterial *mat = scene->mMaterials[m->mMaterialIndex];

	process_material(mat, aiTextureType_DIFFUSE, material, Material::TexDiffuse, dir);
	process_material(mat, aiTextureType_SPECULAR, material, Material::TexSpecular, dir);

	aiColor3D color;
	mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
	material.color_ambient = glm::vec3(color.r, color.g, color.b);
	mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	material.color_diffuse = glm::vec3(color.r, color.g, color.b);
	mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
	material.color_specular = glm::vec3(color.r, color.g, color.b);

	res->set_material(material);
	return res;
}

void ModelLoader::process_material(const aiMaterial *mat, aiTextureType aiType, Material &material, Material::TextureType type, const std::string &path)
{
	AssetLoader loader = AssetLoader::get_instance();
	for (uint i = 0; i < mat->GetTextureCount(aiType); ++i)
	{
		aiString relPath;
		mat->GetTexture(aiType, i, &relPath);

		std::string str(relPath.C_Str());
		std::replace(str.begin(), str.end(), '\\', '/');
		if (str.at(0) != '/')
			str.insert(0, "/");

		std::cout << relPath.C_Str() << std::endl;
		std::shared_ptr<Texture> texture = loader.load<TextureLoader>(std::string(path + str));
		material.textures.push_back(pair<Material::TextureType, std::shared_ptr<Texture>>(type, texture));
	}
}
