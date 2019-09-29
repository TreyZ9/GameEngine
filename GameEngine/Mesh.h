#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "TessellationShader.h"
#include "ReflectionShader.h"
#include "ShaderProgram.h"
#include "NormalShader.h"
#include "StaticShader.h"
#include "Texture.h"
#include "Shader.h"
#include "Vertex.h"

#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

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

	Mesh();

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	// type = "diffuse|specular|normal|height|displacement"
	void applyTexture(const std::string& filename, std::string type, float gamma = false);

	void bindTextures(ShaderProgram* shader);

	void draw(Shader shader, glm::mat4 transformationMatrix, Light unusedLight);

	void draw(StaticShader shader, glm::mat4 transformationMatrix, Light light);

	void draw(NormalShader shader, glm::mat4 transformationMatrix, Light light);

	void draw(TessellationShader shader, glm::mat4 transformationMatrix, Light light);

	void draw(ReflectionShader shader, glm::mat4 transformationMatrix, Light light);

private:
	GLuint VBO, EBO;

	void setupMesh();
};
