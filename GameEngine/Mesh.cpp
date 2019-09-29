
#include "Mesh.h"

#include "AssetLoader.h"
#include "Config.h"
#include "Camera.h"

Mesh::Mesh() {}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) : vertices(vertices), indices(indices), textures(textures)
{
	setupMesh();
}

// type = "diffuse|specular|normal|height|displacement"
void Mesh::applyTexture(const std::string& filename, std::string type, float gamma)
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

void Mesh::bindTextures(ShaderProgram* shader)
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

void Mesh::draw(Shader shader, glm::mat4 transformationMatrix, Light unusedLight)
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

void Mesh::draw(StaticShader shader, glm::mat4 transformationMatrix, Light light)
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

void Mesh::draw(NormalShader shader, glm::mat4 transformationMatrix, Light light)
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

void Mesh::draw(TessellationShader shader, glm::mat4 transformationMatrix, Light light)
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

void Mesh::draw(ReflectionShader shader, glm::mat4 transformationMatrix, Light light)
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

void Mesh::setupMesh()
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
