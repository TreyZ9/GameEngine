
#include "Mesh.h"

#include "OpenGLFunctions.h"
#include "Config.h"
#include "Loader.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, Material mat) :
	vertices(vertices), indices(indices), textures(textures), mat(mat)
{
	this->setupMesh();
}

void Mesh::setupMesh()
{
	glCall(glGenVertexArrays, 1, &this->vao);
	glCall(glGenBuffers, 1, &this->vbo);
	glCall(glGenBuffers, 1, &this->ebo);
	glCall(glGenBuffers, 1, &this->uniformBlockIndex);

	glCall(glBindVertexArray, this->vao);

	glCall(glBindBuffer, GL_ARRAY_BUFFER, this->vbo);

	glCall(glBufferData, GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex) + sizeof(this->mat), &this->vertices[0], GL_STATIC_DRAW);
	glCall(glBindBuffer, GL_UNIFORM_BUFFER, this->uniformBlockIndex);
	glCall(glBufferData, GL_UNIFORM_BUFFER, sizeof(this->mat), (void*)(&this->mat), GL_STATIC_DRAW);

	glCall(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, this->ebo);
	glCall(glBufferData, GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

	Loader::createAttibutePointer(0, 3, sizeof(Vertex), (void*)0);
	Loader::createAttibutePointer(1, 2, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	Loader::createAttibutePointer(2, 3, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	Loader::createAttibutePointer(3, 3, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	Loader::createAttibutePointer(4, 3, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
}

void Mesh::bindTextures(Shader& shader)
{
	unsigned int diffuseNr = 0;
	unsigned int specularNr = 0;
	unsigned int normalNr = 0;
	unsigned int heightNr = 0;
	unsigned int displacementNr = 0;
	unsigned int cubeMapNr = 0;

	for (unsigned int i = 0; i < this->textures.size(); i++)
	{
		glCall(glActiveTexture, GL_TEXTURE0 + i);

		std::string number;
		std::string name = this->textures[i].Type;
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

		glCall(glUniform1i, glGetUniformLocation(shader.getProgramID(), (name + number).c_str()), i);
		if (name == "texture_cubeMap")
			glCall(glBindTexture, GL_TEXTURE_CUBE_MAP, textures[i].ID);
		else
			glCall(glBindTexture, GL_TEXTURE_2D, textures[i].ID);
	}
}

void Mesh::draw(Shader shader, glm::mat4 transformationMatrix)
{
	this->bindTextures(shader);

	shader.loadTransformationMatrix(transformationMatrix);
	glm::mat4 projectionMatrix = glm::perspective(Config::Display::FOV, (float)Config::Display::WIDTH / 
		(float)Config::Display::HEIGHT, Config::Display::NEAR_PLANE, Config::Display::FAR_PLANE);
	shader.loadProjectionMatrix(projectionMatrix);
	shader.loadViewMatrix();

	glCall(glBindVertexArray, this->vao);
	glCall(glBindBufferRange, GL_UNIFORM_BUFFER, 0, this->uniformBlockIndex, 0, sizeof(Material));
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glCall(glBindVertexArray, 0);
	glCall(glActiveTexture, GL_TEXTURE0);
}
