#pragma once

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Texture.h"
#include "Mesh.h"

class Model
{
public:
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection = false;

	glm::mat4 transformationMatrix;
	glm::vec3 position;
	glm::vec3 rotation;
	float scale;

	Model();

	Model(std::string const& path, glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), float scale = 1.0f, bool gamma = false);

	template <typename ShaderType> void draw(ShaderType shader, Light light = Light());

	void setPosition(glm::vec3 position);
	void setRotation(glm::vec3 rotation);
	void setScale(float scale);

	void increasePosition(glm::vec3 position);
	void increaseRotation(glm::vec3 rotation);
	void increaseScale(float scale);

	void updateTransformationMatrix();

private:
	void loadModel(std::string const& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

template <typename ShaderType> void Model::draw(ShaderType shader, Light light)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].draw(shader, this->transformationMatrix, light);
}
