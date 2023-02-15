#pragma once

#include "TextShader.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <string>
#include <vector>
#include <map>

#include "TextRenderer.h"
#include "Config.h"

class FpsModel
{
private:
	std::vector<int> fpsBuffer = std::vector<int>(Config::Display::FPS_BUFFER_SIZE, 60);
	std::string fps = "60";

public:
	FpsModel() = default;

	~FpsModel() = default;

	void update();

	void render(TextShader shader, TextRenderer textRenderer);
};
