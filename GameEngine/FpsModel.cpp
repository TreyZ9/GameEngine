
#include "FpsModel.h"

#include <glm/gtc/matrix_transform.hpp>

#include <sstream>

#include "OpenGLFunctions.h"
#include "Config.h"
#include "Vertex.h"
#include "Loader.h"
#include "Maths.h"

void FpsModel::update(const Display& display)
{
	int fps = (int)(1 / display.getFrameDelta());
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

void FpsModel::render(const Display& display, TextShader shader, TextRenderer textRenderer)
{
	textRenderer.drawTextOnHUD(display, shader, fps, display.getResolution(), glm::vec2(30.0f), glm::vec3(0.0f, 1.0f, 0.0f), "left", "topright");
}
