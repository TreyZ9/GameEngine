#pragma once

#include <glad/glad.h>

#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ReflectionShader.h"
#include "BSDFShader.h"
#include "Shader.h"
#include "Loader.h"
#include "Mesh.h"

class Model
{
private:
	void loadModel(const std::string& path);

	void processNode(aiNode* node, const aiScene* scene);

	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

public:
	std::vector<Texture> texturesLoaded;
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;

	Model(const std::string& path, bool gamma = false);

	void draw(Shader shader, const glm::mat4& transformationMatrix);

	void draw(BSDFShader shader, const glm::mat4& transformationMatrix);

	void draw(ReflectionShader shader, const glm::mat4& transformationMatrix, const std::vector<Light>& lights);

	void setCubeMap(Texture cubeMapTexture);
};
