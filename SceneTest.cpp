#include "SceneTest.h"
#include "Game.h"

const glm::vec3 SceneTest::kLightDirection = glm::normalize(glm::vec3(0.0, -4.0, -1.0));
const float SceneTest::kAmbientLight = 0.4f;

SceneTest::SceneTest() {}
SceneTest::~SceneTest() {}

void SceneTest::initScene() {
	Scene::initScene();

	float FOV = (float)M_PI/3.f;
	float ar = (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT;
	float znear = 0.1f;
	float zfar  = 100.f;
	mProjectionMatrix = glm::perspective(FOV, ar, znear, zfar);


	glm::vec3 OBS = glm::vec3(0.f, 8.f, 32.f);
	glm::vec3 VRP = glm::vec3(0.f, 0.f, -64.f);
	glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
	mViewMatrix = glm::lookAt(OBS, VRP, up);

	mModel.init();
}

void SceneTest::updateScene(int deltaTime) {
  Scene::updateScene(deltaTime);

  if (Game::instance().getKeyPressed(27)) // Escape
    Game::instance().stop();
}

void SceneTest::renderScene() {
	Scene::renderScene();

	mTexProgram->setUniform3f("lightDir", kLightDirection.x, kLightDirection.y, kLightDirection.z);
	mTexProgram->setUniform1f("ambientColor", kAmbientLight);

	glEnable(GL_CULL_FACE);
	mModel.render();
	glDisable(GL_CULL_FACE);
}