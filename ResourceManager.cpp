#include "ResourceManager.h"

const std::string ResourceManager::kTexturePath = "images/";
const std::string ResourceManager::kMusicPath = "music/";

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() {
	std::map<std::string, Texture*>::iterator it = mTextures.begin();
	Texture* texture;

	while (it != mTextures.end()) {
		texture = it->second;
		delete texture;
		texture = nullptr;

		++it;
	}

	mTextures.clear();
}

Texture* ResourceManager::getTexture(const std::string& name) {
	std::map<std::string, Texture*>::iterator it = mTextures.find(name);
	Texture* texture = it->second;

	/* Texture does not exist */
	if (it == mTextures.end()) {
		texture = new Texture();
		texture->loadFromFile(kTexturePath + name, TEXTURE_PIXEL_FORMAT_RGBA);

		mTextures[name] = texture;
	}

	return texture;
}

void ResourceManager::setBackgroundMusic(const std::string& name) {
	if (name == mCurrentBackgroundMusic) return;

	mCurrentBackgroundMusic = name;
	mBackgroundMusic.openFromFile(kMusicPath + name);
	mBackgroundMusic.play();
}