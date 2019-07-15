#pragma once

#include "ReflectionShader.h"
#include "Shader.h"
#include "Errors.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
	float gamma;
};

struct AssetLoader
{
public:
	static std::map<std::string, Texture> textures;
	static std::vector<GLuint> vaos;
	static std::vector<GLuint> vbos;
	static std::vector<GLuint> ebos;

	static Texture loadTexture(const std::string &filename, const std::string &type, bool gamma = false);
	static Texture loadCubeMap(const std::string &directory);
	static void cleanUp();

	template <typename dataType> static void storeDataInAttributeList(int attributeNumber, int coordinateSize, std::vector<dataType> data);
	template <typename dataSize_t, typename offset_t> static void createAttibutePointer(int attributeNumber, int coordinateSize, dataSize_t dataType, offset_t offset);
	
	static GLuint bindIndicesArray(std::vector<GLuint> indices);
	static GLuint createVAO();
	static void unbindVAO();
};

std::map<std::string, Texture> AssetLoader::textures = {};
std::vector<GLuint> AssetLoader::vaos = {};
std::vector<GLuint> AssetLoader::vbos = {};
std::vector<GLuint> AssetLoader::ebos = {};

Texture AssetLoader::loadTexture(const std::string &filename, const std::string &type, bool gamma)
{
	if (AssetLoader::textures.count(filename) == 0)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
			
			Debug::fileLoad(filename, type, true);
		}
		else
		{
			Debug::fileLoad(filename, type, false);
			stbi_image_free(data);
		}

		Texture texture;
		texture.id = textureID;
		texture.type = type;
		texture.path = filename;
		texture.gamma = gamma;
		AssetLoader::textures.insert(std::pair<std::string, Texture>(filename, texture));

		return texture;
	}
	else
		return AssetLoader::textures[filename];
}

Texture AssetLoader::loadCubeMap(const std::string &directory)
{
	if (AssetLoader::textures.count(directory) == 0)
	{
		std::vector<std::string> faces = { directory + "/right.png", directory + "/left.png", directory + "/top.png",
			directory + "/bottom.png", directory + "/back.png", directory + "/front.png" };

		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		stbi_set_flip_vertically_on_load(false);
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			int width, height, nrChannels;
			unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);

				Debug::fileLoad(faces[i], "cubeMap", true);
			}
			else
			{
				stbi_image_free(data);
				Debug::fileLoad(faces[i], "cubeMap", false);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		Texture texture;
		texture.id = textureID;
		texture.type = "cubeMap";
		texture.path = directory;
		texture.gamma = 0.0f;
		AssetLoader::textures.insert(std::pair<std::string, Texture>(directory, texture));

		return texture;
	}
	else
		return AssetLoader::textures[directory];
}

void AssetLoader::cleanUp()
{
	
	for (GLuint vao : AssetLoader::vaos)
		glDeleteVertexArrays(1, &vao);
	for (GLuint vbo : AssetLoader::vbos)
		glDeleteBuffers(1, &vbo);
	for (GLuint ebo : AssetLoader::ebos)
		glDeleteBuffers(1, &ebo);
	for (auto const &pair : AssetLoader::textures)
		glDeleteTextures(1, &pair.second.id);
}

template <typename dataType_t> void AssetLoader::storeDataInAttributeList(int attributeNumber, int coordinateSize, std::vector<dataType_t> data) 
{
	GLuint VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(dataType_t), &data[0], GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE, 0, 0);
}

template <typename dataSize_t, typename offset_t> void AssetLoader::createAttibutePointer(int attributeNumber, int coordinateSize, dataSize_t dataSize, offset_t offset)
{
	glEnableVertexAttribArray(attributeNumber);
	glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE, dataSize, offset);
}

GLuint AssetLoader::bindIndicesArray(std::vector<GLuint> indices) {
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), &indices[0], GL_STATIC_DRAW);
	return EBO;
}

GLuint AssetLoader::createVAO() {
	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	return vaoID;
}

void AssetLoader::unbindVAO() { glBindVertexArray(0); }

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
