#pragma once

#include "ReflectionShader.h"
#include "AssetLoader.h"
#include "Texture.h"
#include "Shader.h"
#include "Errors.h"
#include "Vertex.h"

class Mesh {
public:
	std::vector<GLuint> vaos;
	std::vector<GLuint> vbos;
	std::vector<GLuint> ebos;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	float gamma = 1.0f;
	float useSpecularMap = false;
	GLuint VAO;

	Mesh() {}
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		setupMesh();
	}

	// type = "diffuse|specular|normal|height|displacement"
	void applyTexture(const std::string &filename, std::string type, float gamma = false) 
	{ 
		if (type == "cubeMap")
			textures.push_back(AssetLoader::loadCubeMap(filename));
		else if (type == "specular")
		{
			textures.push_back(AssetLoader::loadTexture(filename, type, gamma));
			this->useSpecularMap = true;
		}
		else
			textures.push_back(AssetLoader::loadTexture(filename, type, gamma)); 
	}

	void bindTextures(ShaderProgram *shader)
	{
		unsigned int diffuseNr = 0;
		unsigned int specularNr = 0;
		unsigned int normalNr = 0;
		unsigned int heightNr = 0;
		unsigned int displacementNr = 0;
		unsigned int cubeMapNr = 0;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			std::string number;
			std::string name = "texture_" + textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);
			else if (name == "texture_normal")
				number = std::to_string(normalNr++);
			else if (name == "texture_height")
				number = std::to_string(heightNr++);
			else if (name == "texture_displacement")
				number = std::to_string(displacementNr++);
			else if (name == "texture_cubeMap")
				number = std::to_string(cubeMapNr++);

			if (name == "texture_cubeMap")
			{ 
				glUniform1i(glGetUniformLocation(shader->getProgramID(), (name + number).c_str()), i);
				glBindTexture(GL_TEXTURE_CUBE_MAP, textures[i].id);
			}
			else
			{
				glUniform1i(glGetUniformLocation(shader->getProgramID(), (name + number).c_str()), i);
				glBindTexture(GL_TEXTURE_2D, textures[i].id);
			}
		}
	}

	void draw(Shader shader, glm::mat4 transformationMatrix, Light unusedLight)
	{
		bindTextures(&shader);

		shader.loadTransformationMatrix(transformationMatrix);
		glm::mat4 projectionMatrix = glm::perspective(Config::Display::FOV, (float)Config::Display::WIDTH / (float)Config::Display::HEIGHT, Config::Display::NEAR_PLANE, Config::Display::FAR_PLANE);
		shader.loadProjectionMatrix(projectionMatrix);
		shader.loadViewMatrix();

		glBindVertexArray(this->VAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(4);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}

	void draw(StaticShader shader, glm::mat4 transformationMatrix, Light light)
	{
		bindTextures(&shader);

		shader.loadTransformationMatrix(transformationMatrix);
		glm::mat4 projectionMatrix = glm::perspective(Config::Display::FOV, (float)Config::Display::WIDTH / (float)Config::Display::HEIGHT, Config::Display::NEAR_PLANE, Config::Display::FAR_PLANE);
		shader.loadProjectionMatrix(projectionMatrix);
		shader.loadViewMatrix();
		shader.loadGamma(this->gamma);
		shader.loadLight(light);

		glBindVertexArray(this->VAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);

		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(4);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}

	void draw(NormalShader shader, glm::mat4 transformationMatrix, Light light)
	{
		bindTextures(&shader);

		shader.loadTransformationMatrix(transformationMatrix);
		glm::mat4 projectionMatrix = glm::perspective(Config::Display::FOV, (float)Config::Display::WIDTH / (float)Config::Display::HEIGHT, Config::Display::NEAR_PLANE, Config::Display::FAR_PLANE);
		shader.loadProjectionMatrix(projectionMatrix);
		shader.loadViewMatrix();
		shader.loadGamma(this->gamma);
		shader.loadLight(light);
		shader.loadCameraPosition(Camera::position);

		glBindVertexArray(this->VAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);

		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(4);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}

	void draw(TessellationShader shader, glm::mat4 transformationMatrix, Light light)
	{
		bindTextures(&shader);

		shader.loadTransformationMatrix(transformationMatrix);
		glm::mat4 projectionMatrix = glm::perspective(Config::Display::FOV, (float)Config::Display::WIDTH / (float)Config::Display::HEIGHT, Config::Display::NEAR_PLANE, Config::Display::FAR_PLANE);
		shader.loadProjectionMatrix(projectionMatrix);
		shader.loadViewMatrix();
		shader.loadLight(light);
		shader.loadGamma(this->gamma);
		shader.loadBlackPoint(0.03f);
		shader.loadCameraPosition(Camera::position);

		glBindVertexArray(this->VAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		
		glPatchParameteri(GL_PATCH_VERTICES, 3);
		glDrawElements(GL_PATCHES, this->indices.size(), GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(4);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}

	void draw(ReflectionShader shader, glm::mat4 transformationMatrix, Light light)
	{
		bindTextures(&shader);

		shader.loadTransformationMatrix(transformationMatrix);
		glm::mat4 projectionMatrix = glm::perspective(Config::Display::FOV, (float)Config::Display::WIDTH / (float)Config::Display::HEIGHT, Config::Display::NEAR_PLANE, Config::Display::FAR_PLANE);
		shader.loadProjectionMatrix(projectionMatrix);
		shader.loadViewMatrix();
		shader.loadGamma(this->gamma);
		shader.loadLight(light);
		shader.loadCameraPosition(Camera::position);
		shader.setUseSpecularMap(this->useSpecularMap);

		glBindVertexArray(this->VAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);

		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(4);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}

private:
	GLuint VBO, EBO;

	void setupMesh()
	{
		VAO = AssetLoader::createVAO();
		
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		EBO = AssetLoader::bindIndicesArray(indices);

		AssetLoader::createAttibutePointer(0, 3, sizeof(Vertex), (void*)0);
		AssetLoader::createAttibutePointer(1, 2, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		AssetLoader::createAttibutePointer(2, 3, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		AssetLoader::createAttibutePointer(3, 3, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		AssetLoader::createAttibutePointer(4, 3, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

		glBindVertexArray(0);

		AssetLoader::vaos.push_back(VAO);
		AssetLoader::vbos.push_back(VBO);
		AssetLoader::ebos.push_back(EBO);
	}
};

class Model
{
public:
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;

	glm::mat4 transformationMatrix;
	glm::vec3 position;
	glm::vec3 rotation;
	float scale;

	Model() {}

	Model(std::string const &path, glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), 
		float scale = 1.0f, bool gamma = false) : gammaCorrection(gamma)
	{
		loadModel(path);

		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
		updateTransformationMatrix();
	}
	
	template <typename ShaderType> void draw(ShaderType shader, Light light = Light())
	{
		for (unsigned int i = 0; i < meshes.size(); i++)
			meshes[i].draw(shader, this->transformationMatrix, light);
	}

	void setPosition(glm::vec3 position) { this->position = position; updateTransformationMatrix(); }
	void setRotation(glm::vec3 rotation) { this->rotation = rotation; updateTransformationMatrix(); }
	void setScale(float scale) { this->scale = scale; updateTransformationMatrix(); }

	void increasePosition(glm::vec3 position) { this->position += position; updateTransformationMatrix(); }
	void increaseRotation(glm::vec3 rotation) { this->rotation += rotation; updateTransformationMatrix(); }
	void increaseScale(float scale) { this->scale += scale; updateTransformationMatrix(); }

	void updateTransformationMatrix()
	{
		Maths::createTransformationMatrix(this->transformationMatrix, this->position,
			this->rotation.x, this->rotation.y, this->rotation.z, this->scale);
	}

private:
	void loadModel(std::string const &path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return;
		}

		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}

	void processNode(aiNode *node, const aiScene *scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}

	}

	Mesh processMesh(aiMesh *mesh, const aiScene *scene)
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
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);

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
				indices.push_back(face.mIndices[j]);
		}

		aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		std::vector<Texture> displacementMaps = loadMaterialTextures(material, aiTextureType_DISPLACEMENT, "texture_displacement");
		textures.insert(textures.end(), displacementMaps.begin(), displacementMaps.end());

		for (Texture texture : textures)
		{
			std::cout << texture.path << std::endl;
		}

		return Mesh(vertices, indices, textures);
	}

	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
	{
		std::vector<Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			textures.push_back(AssetLoader::loadTexture(str.C_Str() + '/' + this->directory, typeName));
		}
		return textures;
	}
};
