#include "SceneTest.h"
#include "Game.h"

const glm::vec3 SceneTest::kLightDirection = glm::normalize(glm::vec3(0.0, -4.0, -1.0));
const float SceneTest::kAmbientLight = 0.4f;

SceneTest::SceneTest()
	: mModel(mTexProgram), mCursor(mModel, mTexProgram) {}

SceneTest::~SceneTest() {
	if (mButtonPlus != nullptr)
		delete mButtonPlus;
	if (mButtonMinus != nullptr)
		delete mButtonMinus;
}

void SceneTest::init() {
	Scene::init();

	float fov = M_PI / 3.0f;
	float width = SCREEN_WIDTH;
	float height = SCREEN_HEIGHT;
	mProjectionMatrix = glm::perspective(fov, width/height, 0.1f, 100.0f);

	glm::vec3 OBS = glm::vec3(0.0f, 0.0f, 10.0f);
	glm::vec3 VRP = glm::vec3(0.0f, 0.0f, -5.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	mViewMatrix = glm::lookAt(OBS, VRP, up);

	mModel.init();
	mModel.loadFromFile("models/test3.txt");

	mTexButtonPlus.loadFromFile("images/button_plus.png", TEXTURE_PIXEL_FORMAT_RGBA);
	mTexButtonMinus.loadFromFile("images/button_minus.png", TEXTURE_PIXEL_FORMAT_RGBA);
	
	mButtonPlus = new Button(
		glm::vec2(16, 16),
		glm::vec2(1.0f, 1.0f),
		&mTexButtonPlus,
		&mGuiProgram
	);
	mButtonPlus->setOnClick([this](){
		mModel.rotateY(M_PI / 4.0f);
	});

	mButtonMinus = new Button(
		glm::vec2(16, 16),
		glm::vec2(1.0f, 1.0f),
		&mTexButtonMinus,
		&mGuiProgram
	);
	mButtonMinus->setOnClick([this](){
		mModel.rotateY(M_PI / -4.0f);
	});
	mButtonMinus->move(glm::vec2(64.0f, 0.0f));

	mCursor.init();
}

void SceneTest::update(int deltaTime) {
	Scene::update(deltaTime);
	mModel.update(deltaTime);
	mButtonPlus->update(deltaTime);
	mButtonMinus->update(deltaTime);
}

void SceneTest::render() {
	Scene::render();
	mTexProgram.setUniform3f("lightDir", kLightDirection.x, kLightDirection.y, kLightDirection.z);
	mTexProgram.setUniform1f("ambientColor", kAmbientLight);
	mModel.render();
	mCursor.render();
	glDisable(GL_CULL_FACE);

	glm::mat4 auxProject = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	glm::mat4 auxView = glm::mat4(1.0f);
	glm::mat4 auxModel = glm::mat4(1.0f);

	mGuiProgram.use();
	mGuiProgram.setUniformMatrix4f("PM", auxProject);
	mGuiProgram.setUniformMatrix4f("VM", auxView);
	mGuiProgram.setUniformMatrix4f("modelview", auxModel);
	mGuiProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	mGuiProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	mButtonPlus->render();
	mButtonMinus->render();
}