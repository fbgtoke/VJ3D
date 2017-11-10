#ifndef _SCENE_MENU_INCLUDE
#define _SCENE_MENU_INCLUDE

#include "Scene.h"
#include "Sprite.h"

class SceneMenu : public Scene {
public:
	SceneMenu();
	~SceneMenu() override;

private:
	Sprite* mBackground;

	void initScene() override;
	void updateScene(int deltaTime) override;
	void renderScene() override;

	void initGUI() override;
	void updateGUI(int deltaTime) override;
	void renderGUI() override;	
};

#endif // _SCENE_MENU_INCLUDE