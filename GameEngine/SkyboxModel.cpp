
#include "SkyboxModel.h"

#include "AssetLoader.h"

#include "Camera.h"
#include "Config.h"
#include "Maths.h"

SkyboxModel::SkyboxModel(const std::string& directory)
{
	this->vao = AssetLoader::createVAO();

	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &this->vertices[0], GL_STATIC_DRAW);

	AssetLoader::createAttibutePointer(0, 3, sizeof(float) * 3, (void*)0);

	glBindVertexArray(0);

	AssetLoader::vaos.push_back(this->vao);
	AssetLoader::vbos.push_back(this->vbo);

	this->texture = AssetLoader::loadCubeMap(directory);
}

SkyboxModel::~SkyboxModel() {}

void SkyboxModel::draw(SkyboxShader shader)
{
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shader.getProgramID(), "texture_cubeMap0"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);

	glm::mat4 projectionMatrix = glm::perspective(Config::Display::FOV,
		(float)Config::Display::WIDTH / (float)Config::Display::HEIGHT,
		Config::Display::NEAR_PLANE, Config::Display::FAR_PLANE);
	shader.loadProjectionMatrix(projectionMatrix);

	glm::mat4 viewMatrix;
	Maths::createTransformationMatrix(viewMatrix, glm::vec3(0.0f), Camera::pitch, Camera::yaw, Camera::roll, 1.0f);
	shader.loadViewMatrix(viewMatrix);

	glBindVertexArray(this->vao);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei) vertices.size() / 3);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);

}
