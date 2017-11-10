#ifndef _SCENE_TEST_INCLUDE
#define _SCENE_TEST_INCLUDE

#include "Scene.h"
#include "Model.h"

#include "Cursor.h"

#include "Button.h"
#include "Texture.h"

class SceneTest : public Scene {
public:
	SceneTest();
	~SceneTest() final;

private:
	static const glm::vec3 kLightDirection;
	static const float kAmbientLight;
	Model mModel;

	Button* mButtonLeft;
	Button* mButtonRight;
	Button* mButtonUp;
	Button* mButtonDown;
	Button* mButtonRotateClockwise;
	Button* mButtonRotateCounterclockwise;
	Button* mButtonUpZ;
	Button* mButtonDownZ;

	Button* mButtonAlphaPlus;
	Button* mButtonAlphaMinus;
	Button* mButtonRedPlus;
	Button* mButtonRedMinus;
	Button* mButtonGreenPlus;
	Button* mButtonGreenMinus;
	Button* mButtonBluePlus;
	Button* mButtonBlueMinus;

	Cursor mCursor;

	void initScene() final;
	void updateScene(int deltaTime) final;
	void renderScene() final;

	void initGUI() override;
	void updateGUI(int deltaTime) override;
	void renderGUI() override;
};

#endif // _SCENE_TEST_INCLUDE