
#include <glad\glad.h>
#include <glfw\glfw3.h>

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

#include "stb_image.h"

#include "TessellationShader.h"
#include "ReflectionShader.h"
#include "DisplayManager.h"
#include "StaticShader.h"
#include "NormalShader.h"
#include "SkyboxShader.h"
#include "SkyboxModel.h"
#include "AssetLoader.h"
#include "ModelLoader.h"
#include "FPSShader.h"
#include "FpsModel.h"
#include "Texture.h"
#include "Vertex.h"
#include "Shader.h"
#include "Config.h"
#include "Camera.h"
#include "Debug.h"
#include "Maths.h"
#include "Model.h"
#include "Light.h"
#include "Mesh.h"

const float ROTATE_SPEED = 100.0f;
const float MOVE_SPEED = 100.0f;

int main() {
	DisplayManager::createDisplay(Config::Display::WIDTH, Config::Display::HEIGHT);

	Shader shader = Shader(
		"shaders/shader/shader.vert",
		"shaders/shader/shader.frag");

	StaticShader staticShader = StaticShader(
		"shaders/staticShader/staticShader.vert", 
		"shaders/staticShader/staticShader.frag");

	NormalShader normalShader = NormalShader(
		"shaders/normalShader/normalShader.vert",
		"shaders/normalShader/normalShader.frag");

	TessellationShader tessShader = TessellationShader(
		"shaders/tessellationShader/vertex.vert",
		"shaders/tessellationShader/fragment.frag",
		"shaders/tessellationShader/control.tesc",
		"shaders/tessellationShader/evaluation.tese");

	ReflectionShader reflectionShader = ReflectionShader(
		"shaders/reflectionShader/shader.vert",
		"shaders/reflectionShader/shader.frag");

	SkyboxShader skyboxShader = SkyboxShader(
		"shaders/skyboxShader/shader.vert",
		"shaders/skyboxShader/shader.frag");

	FPSShader fpsShader = FPSShader(
		"shaders/fpsShader/vertexShaderFPS.vert",
		"shaders/fpsShader/fragmentShaderFPS.frag");

	Light light(glm::vec3(60.0f, 100.0f, 100.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	FpsModel fpsModel = FpsModel();

	SkyboxModel skyboxModel("res/skyboxDay");

	std::vector<Model> assimpModels = ModelLoader::loadModels("res/models.scene");
	Model assimpModel = assimpModels[0];

	DisplayManager::hideCursor();
	//DisplayManager::showCursor();

	while (!glfwWindowShouldClose(DisplayManager::window) && !glfwGetKey(DisplayManager::window, GLFW_KEY_ESCAPE)) {
		double xPos, yPos;
		glfwGetCursorPos(DisplayManager::window, &xPos, &yPos);
		Camera::move(xPos - (Config::Display::WIDTH / 2), yPos - (Config::Display::HEIGHT / 2));
		glfwSetCursorPos(DisplayManager::window, Config::Display::WIDTH / 2, Config::Display::HEIGHT / 2);

		// Model Manipulation
		if (glfwGetKey(DisplayManager::window, GLFW_KEY_E))
			Config::Display::FOV += DisplayManager::DELTA;
			//gamma += DisplayManager::DELTA;
		if (glfwGetKey(DisplayManager::window, GLFW_KEY_Q))
			Config::Display::FOV -= DisplayManager::DELTA;
			//gamma -= DisplayManager::DELTA;

		assimpModel.increaseRotation(glm::vec3(0.0f, 0.1f, 0.0f));

		// Clear Screen Buffers
		glClearColor(0, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Shader Cycle
		shader.start();
		assimpModel.draw(shader);
		shader.stop();

		// Static Shader Cycle
		staticShader.start();
		assimpModel.increasePosition(glm::vec3(-30.0f, 0.0f, 0.0f));
		assimpModel.draw(staticShader, light);
		assimpModel.increasePosition(glm::vec3( 30.0f, 0.0f, 0.0f));
		staticShader.stop();

		// Normal Shader Cycle
		normalShader.start();
		assimpModel.increasePosition(glm::vec3(-60.0f, 0.0f, 0.0f));
		assimpModel.draw(normalShader, light);
		assimpModel.increasePosition(glm::vec3( 60.0f, 0.0f, 0.0f));
		normalShader.stop();

		// Tessellation Shader Cycle
		tessShader.start();
		assimpModel.increasePosition(glm::vec3(-90.0f, 0.0f, 0.0f));
		assimpModel.draw(tessShader, light);
		assimpModel.increasePosition(glm::vec3( 90.0f, 0.0f, 0.0f));
		tessShader.stop();

		// Reflection Shader Cycle
		reflectionShader.start();
		assimpModel.increasePosition(glm::vec3(-120.0f, 0.0f, 0.0f));
		assimpModel.draw(reflectionShader, light);
		assimpModel.increasePosition(glm::vec3(120.0f, 0.0f, 0.0f));
		reflectionShader.stop();

		// Skybox Shader Cycle
		skyboxShader.start();
		skyboxModel.draw(skyboxShader);
		skyboxShader.stop();

		// FPS Shader Cycle
		fpsModel.update();
		fpsModel.render(fpsShader);

		// Show Display Buffer
		DisplayManager::updateDisplay();
	}

	shader.cleanUp();
	staticShader.cleanUp();
	normalShader.cleanUp();
	tessShader.cleanUp();
	fpsShader.cleanUp();
	AssetLoader::cleanUp();
	DisplayManager::closeDisplay();

	system("pause");
}
