
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
	glCall(glGenVertexArrays, 1, &this->vao);
	glCall(glGenBuffers, 1, &this->vbo);
	glCall(glBindVertexArray, this->vao);
	glCall(glBindBuffer, GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glCall(glEnableVertexAttribArray, 0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glCall(glBindBuffer, GL_ARRAY_BUFFER, 0);
	glCall(glBindVertexArray, 0);
}

FpsModel::~FpsModel() {}

GLuint FpsModel::getTexture(char number, TextRenderer textRenderer)
{
	return textRenderer.getCharacter(number).textureID;
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
	this->fps = "FPS: " + ss.str();
}

void FpsModel::render(FPSShader shader, TextRenderer textRenderer)
{
	float x = 700.0f;
	float y = 570.0f;
	float scale = 0.7f;

	shader.start();

	glm::mat4 projectionMatrix = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
	shader.loadTransformationMatrix(projectionMatrix);
	shader.loadTextColor(glm::vec3(0.0f, 1.0f, 0.0f));

	glCall(glActiveTexture, GL_TEXTURE0);
	glCall(glBindVertexArray, this->vao);

	for (char c : fps)
	{
		Character character = textRenderer.getCharacter(c);

		float xpos = x + character.bearing.x * scale;
		float ypos = y - (character.size.y - character.bearing.y) * scale;

		float w = character.size.x * scale;
		float h = character.size.y * scale;

		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};

		glCall(glBindTexture, GL_TEXTURE_2D, character.textureID);
		glCall(glBindBuffer, GL_ARRAY_BUFFER, this->vbo);
		glCall(glBufferSubData, GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glCall(glBindBuffer, GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (character.advance >> 6) * scale;
	}

	glCall(glBindVertexArray, 0);
	glCall(glBindTexture, GL_TEXTURE_2D, 0);

	shader.stop();
}
