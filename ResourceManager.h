#ifndef _RESOURCE_MANAGER_INCLUDE
#define _RESOURCE_MANAGER_INCLUDE

#include "utils.h"
#include "Texture.h"

class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();

	Texture* getTexture(const std::string& name);

private:
	static const std::string kTexturePath;
	std::map<std::string, Texture*> mTextures;	
};

#endif // _RESOURCE_MANAGER_INCLUDE