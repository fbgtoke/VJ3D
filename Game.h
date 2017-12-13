#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE

#include "utils.h"
#include "Scene.h"
#include "ResourceManager.h"

// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

public:
	Game() {}
	
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

	void scanKeys();
	bool getKeyPressed(int key) const;
	bool getKeyReleased(int key) const;
	bool getSpecialKeyPressed(int key) const;
	bool getSpecialKeyReleased(int key) const;

	glm::ivec2 getMousePosition() const;

	bool getMouseButton() const;
	bool getMouseButtonPressed() const;
	bool getMouseButtonReleased() const;

	void resize(int width, int height);

	void changeScene(Scene::SceneType type);
	Scene* getScene();
	Scene* getBufferedScene();
	
	void stop();

	ResourceManager& getResource();

	void setBackgroundMusic(const std::string& name, float volume = 50.f);

private:
	bool bPlay;                       // Continue to play game?
	Scene* scene;                     // Scene to render
	Scene* bufferedScene;
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time

	bool mKeysCurrent[256];
	bool mKeysPrevious[256];

	bool mSpecialKeysCurrent[256];
	bool mSpecialKeysPrevious[256];

	glm::ivec2 mMousePosition;
	bool mMouseButton;
	bool mMouseButtonCurrent;
	bool mMouseButtonPrevious;

	ResourceManager mResourceManager;
	sf::Music* mBackgroundMusic;
};


#endif // _GAME_INCLUDE


