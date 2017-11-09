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

	void init() final;
	void update(int deltaTime) final;
	void render() final;

private:
	static const glm::vec3 kLightDirection;
	static const float kAmbientLight;
	Model mModel;

	Texture mTexButtonPlus;
	Texture mTexButtonMinus;
	Texture mTexButtonLeft;
	Texture mTexButtonRight;
	Texture mTexButtonUp;
	Texture mTexButtonDown;
	Texture mTexButtonRotateClockwise;
	Texture mTexButtonRotateCounterclockwise;
	Texture mTexButtonUpZ;
	Texture mTexButtonDownZ;

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
};

#endif // _SCENE_TEST_INCLUDE