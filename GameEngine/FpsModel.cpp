
#include "FpsModel.h"

#include "DisplayManager.h"
#include "AssetLoader.h"
#include "Maths.h"

#include <glm/gtc/matrix_transform.hpp>

#include <sstream>

FpsModel::FpsModel()
{
	this->vaoID = AssetLoader::createVAO();
	AssetLoader::storeDataInAttributeList(0, 3, vertices);
	AssetLoader::storeDataInAttributeList(1, 2, textureCoords);
	AssetLoader::bindIndicesArray(indices);
	this->vertexCount = (int) indices.size();

	this->fpsTextures['0'] = AssetLoader::loadTexture("res/fpsTextures/0.png", "diffuse").id;
	this->fpsTextures['1'] = AssetLoader::loadTexture("res/fpsTextures/1.png", "diffuse").id;
	this->fpsTextures['2'] = AssetLoader::loadTexture("res/fpsTextures/2.png", "diffuse").id;
	this->fpsTextures['3'] = AssetLoader::loadTexture("res/fpsTextures/3.png", "diffuse").id;
	this->fpsTextures['4'] = AssetLoader::loadTexture("res/fpsTextures/4.png", "diffuse").id;
	this->fpsTextures['5'] = AssetLoader::loadTexture("res/fpsTextures/5.png", "diffuse").id;
	this->fpsTextures['6'] = AssetLoader::loadTexture("res/fpsTextures/6.png", "diffuse").id;
	this->fpsTextures['7'] = AssetLoader::loadTexture("res/fpsTextures/7.png", "diffuse").id;
	this->fpsTextures['8'] = AssetLoader::loadTexture("res/fpsTextures/8.png", "diffuse").id;
	this->fpsTextures['9'] = AssetLoader::loadTexture("res/fpsTextures/9.png", "diffuse").id;
}

FpsModel::~FpsModel() {}

GLuint FpsModel::getTexture(char number)
{
	return this->fpsTextures[number];
}

void FpsModel::update()
{
	int fps = (int)(1 / DisplayManager::DELTA);
	this->fpsBuffer.insert(this->fpsBuffer.begin(), fps);
	this->fpsBuffer.erase(this->fpsBuffer.begin() + Config::Display::FPS_BUFFER_SIZE);
	fps = 0;
	for (int f : this->fpsBuffer)
		fps += f;
	fps /= Config::Display::FPS_BUFFER_SIZE;
	std::stringstream ss;
	ss << fps;
	this->fps = ss.str();
}

void FpsModel::render(FPSShader shader)
{
	shader.start();
	float x = 0.5f;
	for (int i = 0; i < this->fps.size(); ++i) {
		x += 0.1f;

		glm::mat4 translationMatrix;
		Maths::createTransformationMatrix(translationMatrix, glm::vec3(x, 0.8f, 0.0f), 0.0f, 0.0f, 0.0f, 0.1f);
		shader.loadTransformationMatrix(translationMatrix);

		glBindVertexArray(this->vaoID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->getTexture(this->fps[i]));
		glDrawElements(GL_TRIANGLES, this->vertexCount, GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
	shader.stop();
}
