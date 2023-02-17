#include "Model.h"

#include "stb_image.h"

#include <spdlog/spdlog.h>

#include "OpenGLFunctions.h"

Model::Model(const std::string& path, bool gamma) : gammaCorrection(gamma)
{
	this->loadModel(path);
}

void Model::draw(Shader shader, const glm::mat4& transformationMatrix)
{
	for (unsigned int i = 0; i < this->meshes.size(); i++)
	{
		this->meshes[i].draw(shader, transformationMatrix);
	}
}

void Model::draw(BSDFShader shader, const glm::mat4& transformationMatrix)
{
	for (unsigned int i = 0; i < this->meshes.size(); i++)
	{
		this->meshes[i].draw(shader, transformationMatrix);
	}
}

void Model::draw(ReflectionShader shader, const glm::mat4& transformationMatrix, const std::vector<Light>& lights)
{
	for (unsigned int i = 0; i < this->meshes.size(); i++)
	{
		this->meshes[i].draw(shader, transformationMatrix, lights);
	}
}

void Model::loadModel(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		spdlog::error("Assimp error, {}", importer.GetErrorString());
		return;
	}

	this->directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f);
		}

		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangent = vector;

		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.Bitangent = vector;

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	Material mat;
	aiColor3D color;

	material->Get(AI_MATKEY_COLOR_AMBIENT, color);
	mat.Ka = glm::vec3(color.r, color.g, color.b);
	material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	mat.Kd = glm::vec3(color.r, color.g, color.b);
	material->Get(AI_MATKEY_COLOR_SPECULAR, color);
	mat.Ks = glm::vec3(color.r, color.g, color.b);
	material->Get(AI_MATKEY_COLOR_EMISSIVE, color);
	mat.Ke = glm::vec3(color.r, color.g, color.b);
	material->Get(AI_MATKEY_REFRACTI, mat.Ni);
	material->Get(AI_MATKEY_OPACITY, mat.d);
	material->Get(AI_MATKEY_SHADING_MODEL, mat.illum);

	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	return Mesh(vertices, indices, textures, mat, mesh->mNumFaces);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		bool skip = false;
		for (unsigned int j = 0; j < this->texturesLoaded.size(); j++)
		{
			if (std::strcmp(this->texturesLoaded[j].Path.data(), str.C_Str()) == 0)
			{
				textures.push_back(texturesLoaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			Texture texture = Loader::loadTextureFromPath(str.C_Str(), this->directory, typeName);
			textures.push_back(texture);
			texturesLoaded.push_back(texture);
		}
	}

	return textures;
}

void Model::setCubeMap(Texture cubeMapTexture)
{
	for (Mesh& mesh : this->meshes)
	{
		mesh.setCubeMap(cubeMapTexture);
	}
}