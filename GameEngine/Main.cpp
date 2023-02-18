
// OpenGL
#include <glad\glad.h>
#include <glfw\glfw3.h>

#include <openAL/al.h>
#include <openAL/alc.h>

// Include Libraries
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

#include <spdlog/spdlog.h>

#include "stb_image.h"

// STD
#include <format>

// Headers
#include "TessellationShader.h"
#include "FrameBufferObject.h"
#include "ReflectionShader.h"
#include "OpenALFunctions.h"
#include "OpenGLFunctions.h"
#include "DisplayManager.h"
#include "StatsTracker.h"
#include "NormalShader.h"
#include "SkyboxShader.h"
#include "TextRenderer.h"
#include "SkyboxModel.h"
#include "PhysicsMesh.h"
#include "BSDFShader.h"
#include "TextShader.h"
#include "Listener.h"
#include "Texture.h"
#include "Source.h"
#include "Vertex.h"
#include "Shader.h"
#include "Config.h"
#include "Camera.h"
#include "Maths.h"
#include "Model.h"
#include "Light.h"
#include "Mesh.h"

// Physics
#include "PhysicsManager.h"
#include "PhysicsBox.h"

#include <bullet3/btBulletDynamicsCommon.h>


int main()
{
	spdlog::set_level(spdlog::level::debug);
	spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

	// Loader::loadSceneJSON("Resources/TestScene/test.json");
	Config::loadConfigs("Settings/settings.ini");
	StatsTracker statsTracker = StatsTracker();

	Listener listener = Listener();

	Source source1 = Source("Resources/audio/ambientMono.wav", glm::vec3(-4.25f, 0.125f, -4.25f), glm::vec3(0.0f), 1.0f, 1.0f, 0.0f, 10.0f, 1.0f, AL_FALSE);
	Source source2 = Source("Resources/audio/heavy.wav");

	Display display = Display(1280, 720, "OpenGL Game Engine");
	// Display display2 = Display(1280, 720, "Second Window", display.getWindow());

	BSDFShader bsdfShader = BSDFShader(
		"Shaders/BSDFShader/bsdfShader.vert",
		"Shaders/BSDFShader/bsdfShader.frag");

	ReflectionShader reflectionShader = ReflectionShader(
		"Shaders/ReflectionShader/reflectionShader.vert",
		"Shaders/ReflectionShader/reflectionShader.frag");

	SkyboxShader skyboxShader = SkyboxShader(
		"Shaders/SkyboxShader/skyboxShader.vert",
		"Shaders/SkyboxShader/skyboxShader.frag");

	TextShader textShader = TextShader(
		"Shaders/TextShader/textShader.vert",
		"Shaders/TextShader/textShader.frag");

	TextRenderer textRenderer = TextRenderer();

	std::vector<Light> lights;
	lights.push_back(Light(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));

	SkyboxModel skyboxModel = SkyboxModel("Resources/skyboxDay");

	Model model = Model("Resources/TestScene/Mesh.obj");
	Model barrel = Model("Resources/Crate/crate.obj");
	Model barrel2 = Model("Resources/Crate/crate.obj");
	Texture skyboxTexture = Loader::loadCubeMap("Resources/skyboxDay");
	barrel2.setCubeMap(skyboxTexture);

	display.hideCursor();
	//DisplayManager::showCursor();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	FrameBufferObject fbo = FrameBufferObject();
	FrameBufferObject fbo2 = FrameBufferObject();

	source1.play();

	/* Physics
	Model physicsCubeGround = Model("Resources/CollisionTest/100x10x100_box.obj");
	
	Model physicsCubeDynamic = Model("Resources/CollisionTest/10x10x10_box.obj");
	PhysicsManager physicsManager = PhysicsManager();

	PhysicsBox groundBox = PhysicsBox(btVector3(100, 10, 100), btVector3(0, -10, 0), btScalar(0.0f));
	physicsManager.addCollisionShape(groundBox.getShape(), groundBox.getBody());
	PhysicsBox dynamicBox = PhysicsBox(btVector3(10, 10, 10), btVector3(0, 100, 0), btScalar(1.0f));
	physicsManager.addCollisionShape(dynamicBox.getShape(), dynamicBox.getBody());

	PhysicsMesh dynamicBox = PhysicsMesh("Resources/Cube/cube.obj", btVector3(0, 50, 0));
	physicsManager.addCollisionShape(dynamicBox.getShape(), dynamicBox.getBody());

		physicsManager.stepSimulation(1.0f / 60.0f);
		physicsCubeGround.draw(bsdfShader, glm::mat4(1.0f));
		glm::vec3 position((float)dynamicBox.getPosition().getX(), (float)dynamicBox.getPosition().getY(), (float)dynamicBox.getPosition().getZ());
		glm::mat4 transform;
		Maths::createTransformationMatrix(transform, position, 0, 0, 0, 1);
		physicsCubeDynamic.draw(bsdfShader, transform);
	*/


	// temp vars
	unsigned int mirrorMeshID = 2;
	float yRot = 0.0f;

	spdlog::debug("Starting main loop");
	while (!glfwWindowShouldClose(display.getWindow()) && !glfwGetKey(display.getWindow(), GLFW_KEY_ESCAPE))
	{
		glfwMakeContextCurrent(display.getWindow());

		if (glfwGetKey(display.getWindow(), GLFW_KEY_0))
			source2.play();
		source2.setPosition(Camera::position);

		Camera::move(display);

		listener.updatePosition();

		// physicsManager.stepSimulation(1.0f / 60.0f);

		// Buffered Shader Cycle (Mirror)
		fbo.bind();
		bsdfShader.start();
		model.draw(bsdfShader, glm::mat4(1.0f), display.getProjectionMatrix());
		bsdfShader.stop();

		skyboxShader.start();
		skyboxModel.draw(skyboxShader, display.getProjectionMatrix());
		skyboxShader.stop();
		fbo.unbind(display.getResolution());

		// Clear Screen Buffers
		display.clear();

		// Shader Cycle
		GLuint tempTexture = model.meshes[mirrorMeshID].textures[0].ID;
		model.meshes[mirrorMeshID].textures[0].ID = fbo.textureColorID;

		// ------------------------------
		// BSDF Shader
		// ------------------------------
		bsdfShader.start();
		model.draw(bsdfShader, glm::mat4(1.0f), display.getProjectionMatrix());
		bsdfShader.stop();

		// physicsCubeGround.draw(bsdfShader, glm::mat4(1.0f));
		// glm::vec3 position((float)dynamicBox.getPosition().getX(), (float)dynamicBox.getPosition().getY(), (float)dynamicBox.getPosition().getZ());
		// glm::mat4 transform;
		// Maths::createTransformationMatrix(transform, position, 0, 0, 0, 1);
		// physicsCubeDynamic.draw(bsdfShader, transform);

		bsdfShader.stop();

		// ------------------------------
		// Reflection Shader
		// ------------------------------
		reflectionShader.start();
		glm::mat4 barrelTransformationMatrix;
		Maths::createTransformationMatrix(barrelTransformationMatrix, glm::vec3(-4.25f, 0.85f, 4.5f), 0.0f, yRot, 0.0f, 1.0f);
		barrel.draw(reflectionShader, barrelTransformationMatrix, display.getProjectionMatrix(), lights);
		Maths::createTransformationMatrix(barrelTransformationMatrix, glm::vec3(-4.25f, 1.9f, 4.5f), 0.0f, yRot, 0.0f, 1.0f);
		barrel2.draw(reflectionShader, barrelTransformationMatrix, display.getProjectionMatrix(), lights);
		reflectionShader.stop();

		model.meshes[mirrorMeshID].textures[0].ID = tempTexture;

		// Skybox Shader Cycle
		skyboxShader.start();
		skyboxModel.draw(skyboxShader, display.getProjectionMatrix());
		skyboxShader.stop();

		textRenderer.drawText(display, textShader, "Controls\n--------------------------------------------------------\nW - Move Forward\nS - Move Backward\nA - Move Left\nD - Move Right\nSpace - Move Up\nLShift - Move Down\nESC - Close Window", glm::vec3(-0.0f, 2.9f, -4.82f), glm::vec3(0.0f), glm::vec2(0.2f), glm::vec3(0.0f, 1.0f, 0.0f), Align::center, Origin::top);

		// FPS Shader Cycle
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//fpsModel.update(display);
		//fpsModel.render(display, textShader, textRenderer);
		statsTracker.update(display.getFrameDelta());
		textRenderer.drawTextOnHUD(display, textShader, std::format("FPS:{:d}", statsTracker.getFps()),
			display.getResolution(), glm::vec2(30.0f), glm::vec3(0.0f, 1.0f, 0.0f), Align::right, Origin::topRight);

		// Show Display Buffer
		display.update();

		//glfwMakeContextCurrent(display2.getWindow());
		// display2.clear();
		//display2.update();

		yRot += display.getFrameDelta() * 16.0f;
		if (yRot > 360.0f)
		{
			yRot -= 360.0f;
		}
	}

	fbo.destroy();
	bsdfShader.cleanUp();
	textShader.cleanUp();
	Loader::destroy();
}
