
#include "SkyboxModel.h"

#include "OpenGLFunctions.h"
#include "DisplayManager.h"
#include "Loader.h"
#include "Camera.h"
#include "Maths.h"

SkyboxModel::SkyboxModel(const std::string& directory)
{
	this->vao = Loader::createVAO();

	glCall(glGenBuffers, 1, &this->vbo);
	glCall(glBindBuffer, GL_ARRAY_BUFFER, this->vbo);
	glCall(glBufferData, GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &this->vertices[0], GL_STATIC_DRAW);

	Loader::createAttibutePointer(0, 3, sizeof(float) * 3, (void*)0);

	glCall(glBindVertexArray, 0);

	this->texture = Loader::loadCubeMap(directory);
}

void SkyboxModel::draw(SkyboxShader shader, const glm::mat4& projectionMatrix)
{
	glCall(glActiveTexture, GL_TEXTURE0);
	glCall(glUniform1i, glGetUniformLocation(shader.getProgramID(), "texture_cubeMap0"), 0);
	glCall(glBindTexture, GL_TEXTURE_CUBE_MAP, texture.ID);

	shader.loadProjectionMatrix(projectionMatrix);

	glm::mat4 viewMatrix;
	Maths::createTransformationMatrix(viewMatrix, glm::vec3(0.0f), 
		Camera::rotation.x, Camera::rotation.y, Camera::rotation.z, 1.0f);
	shader.loadViewMatrix(viewMatrix);

	glCall(glBindVertexArray, this->vao);
	glCall(glEnableVertexAttribArray, 0);

	glCall(glDrawArrays, GL_TRIANGLES, 0, (GLsizei) vertices.size() / 3);

	glCall(glDisableVertexAttribArray, 0);
	glCall(glBindVertexArray, 0);
	glCall(glActiveTexture, GL_TEXTURE0);

}
