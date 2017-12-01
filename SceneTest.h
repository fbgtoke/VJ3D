#ifndef SCENE_TEST_INCLUDE
#define SCENE_TEST_INCLUDE

#include "Scene.h"
#include "Model.h"

class SceneTest : public Scene {
public:
	SceneTest();
	~SceneTest() override;

private:
	static const glm::vec3 kLightDirection;
	static const float kAmbientLight;

	void initScene() override;
	void updateScene(int deltaTime) override;
	void renderScene() override;

	Model mModel;
};

#endif // SCENE_TEST_INCLUDE