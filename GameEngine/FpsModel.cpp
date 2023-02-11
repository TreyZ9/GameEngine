
#include "FpsModel.h"

#include "OpenGLFunctions.h"
#include "DisplayManager.h"
#include "Vertex.h"
#include "Loader.h"
#include "Maths.h"

#include <glm/gtc/matrix_transform.hpp>

#include <sstream>

FpsModel::FpsModel() {}

FpsModel::~FpsModel() {}

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

void FpsModel::render(TextShader shader, TextRenderer textRenderer)
{
	textRenderer.drawTextOnHUD(shader, fps, glm::vec2(700.0f, 570.0f), glm::vec2(0.7f), glm::vec3(0.0f, 1.0f, 0.0f));
}
