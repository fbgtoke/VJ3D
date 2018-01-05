#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE

#include "utils.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "InputManager.h"

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

	ResourceManager mResourceManager;
	sf::Music* mBackgroundMusic;
};


#endif // _GAME_INCLUDE


