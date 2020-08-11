
#include "FpsModel.h"

#include "OpenGLFunctions.h"
#include "DisplayManager.h"
#include "Vertex.h"
#include "Loader.h"
#include "Maths.h"

#include <glm/gtc/matrix_transform.hpp>

#include <sstream>

FpsModel::FpsModel()
{
	glCall(glGenVertexArrays, 1, &this->vaoID);
	glCall(glBindVertexArray, this->vaoID);

	Loader::storeDataInAttributeList(0, 3, vertices);
	Loader::storeDataInAttributeList(1, 2, textureCoords);
	Loader::createEBO(indices);
	this->vertexCount = (int) indices.size();

	this->fpsTextures['0'] = Loader::loadTexture("res/fpsTextures/0.png", "diffuse").ID;
	this->fpsTextures['1'] = Loader::loadTexture("res/fpsTextures/1.png", "diffuse").ID;
	this->fpsTextures['2'] = Loader::loadTexture("res/fpsTextures/2.png", "diffuse").ID;
	this->fpsTextures['3'] = Loader::loadTexture("res/fpsTextures/3.png", "diffuse").ID;
	this->fpsTextures['4'] = Loader::loadTexture("res/fpsTextures/4.png", "diffuse").ID;
	this->fpsTextures['5'] = Loader::loadTexture("res/fpsTextures/5.png", "diffuse").ID;
	this->fpsTextures['6'] = Loader::loadTexture("res/fpsTextures/6.png", "diffuse").ID;
	this->fpsTextures['7'] = Loader::loadTexture("res/fpsTextures/7.png", "diffuse").ID;
	this->fpsTextures['8'] = Loader::loadTexture("res/fpsTextures/8.png", "diffuse").ID;
	this->fpsTextures['9'] = Loader::loadTexture("res/fpsTextures/9.png", "diffuse").ID;
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

		glCall(glBindVertexArray, this->vaoID);
		glCall(glEnableVertexAttribArray, 0);
		glCall(glEnableVertexAttribArray, 1);
		glCall(glActiveTexture, GL_TEXTURE0);
		glCall(glBindTexture, GL_TEXTURE_2D, this->getTexture(this->fps[i]));
		glDrawElements(GL_TRIANGLES, this->vertexCount, GL_UNSIGNED_INT, 0); // Currently Incompatible With glCall, Needs Fixed
		glCall(glDisableVertexAttribArray, 1);
		glCall(glDisableVertexAttribArray, 0);
		glCall(glBindVertexArray, 0);
	}
	shader.stop();
}
