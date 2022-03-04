#pragma once

#include <glad/glad.h>

#include <unordered_map>
#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "BSDFShader.h"
#include "Material.h"
#include "Texture.h"
#include "Shader.h"
#include "Vertex.h"

class Mesh
{
private:
	unsigned int vbo = NULL;
	unsigned int ebo = NULL;

	void setupMesh();

	void bindTextures(GLuint programID);

public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	Material mat;
	unsigned int vao = NULL;
	unsigned int uniformBlockIndex;
	unsigned int numFaces;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, Material mat, unsigned int numFaces);

	void draw(Shader shader, glm::mat4 transformationMatrix);

	void draw(BSDFShader shader, glm::mat4 transformationMatrix);
};
