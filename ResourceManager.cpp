#include "ResourceManager.h"
#include "Game.h"

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() {
  for (auto it = mMeshes.begin(); it != mMeshes.end();) {
    //std::cout << "Erasing mesh " + it->first << std::endl;
    delete it->second;
    mMeshes.erase(it++);
  }
  mMeshes.clear();

  for (auto it = mShaders.begin(); it != mShaders.end();) {
    //std::cout << "Erasing shader " + it->first << std::endl;
    it->second->free();
    delete it->second;
    mShaders.erase(it++);
  }
  mShaders.clear();

  for (auto it = mTextures.begin(); it != mTextures.end();) {
    //std::cout << "Erasing texture " + it->first << std::endl;
    delete it->second;
    mTextures.erase(it++);
  }
  mTextures.clear();

  for (auto it = mMusics.begin(); it != mMusics.end();) {
    //std::cout << "Erasing texture " + it->first << std::endl;
    delete it->second;
    mMusics.erase(it++);
  }
  mMusics.clear();

  for (auto it = mSoundBuffers.begin(); it != mSoundBuffers.end();) {
    //std::cout << "Erasing texture " + it->first << std::endl;
    delete it->second;
    mSoundBuffers.erase(it++);
  }
  mSoundBuffers.clear();
}

Mesh* ResourceManager::mesh(const std::string& name) {
	if (mMeshes.count(name) == 0)
		loadMesh(name);

	return mMeshes[name];
}

ShaderProgram* ResourceManager::shader(const std::string& name) {
	if (mShaders.count(name) == 0)
		loadShader(name);

	return mShaders[name];
}

Texture* ResourceManager::texture(const std::string& name, bool fullPath) {
	if (mTextures.count(name) == 0)
		loadTexture(name, fullPath);

	return mTextures[name];
}

sf::Music* ResourceManager::music(const std::string& name) {
  if (mMusics.count(name) == 0)
    loadMusic(name);

  return mMusics[name];
}

sf::SoundBuffer* ResourceManager::soundBuffer(const std::string& name) {
  if (mSoundBuffers.count(name) == 0)
    loadSoundBuffer(name);

  return mSoundBuffers[name];
}

int ResourceManager::Int(const std::string& name) {
  if (mConstantsInt.count(name) == 0) {
    std::cout << "Tried to load undefined constant: " << name << std::endl;
    return 0.f;
  }

  return mConstantsInt[name];
}

float ResourceManager::Float(const std::string& name) {
  if (mConstantsFloat.count(name) == 0) {
    std::cout << "Tried to load undefined constant: " << name << std::endl;
    return 0.f;
  }

  return mConstantsFloat[name];
}

void ResourceManager::setInt(const std::string& name, int value) {
  mConstantsInt[name] = value;
}

void ResourceManager::loadMesh(const std::string& name) {
  Mesh* mesh = new Mesh();

  ObjReader reader;
  reader.loadFromFile("models/" + name);

  float *vertices, *normals, *texcoords;
  size_t nvertices, nnormals, ntexcoords;

  reader.getVertices(vertices, nvertices);
  reader.getNormals(normals, nnormals);
  reader.getTexCoords(texcoords, ntexcoords);
  
  mesh->setVertices(vertices, nvertices);
  mesh->setNormals(normals, nnormals);
  mesh->setTexCoords(texcoords, ntexcoords);
  mesh->initVAO();

  mMeshes[name] = mesh;
}

void ResourceManager::loadShader(const std::string& name) {
	ShaderProgram* shaderProgram = new ShaderProgram();
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/" + name + ".vert");
	if(!vShader.isCompiled())
	{
		cout << "3D Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/" + name + ".frag");
	if(!fShader.isCompiled())
	{
		cout << "3D Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	shaderProgram->init();
	shaderProgram->addShader(vShader);
	shaderProgram->addShader(fShader);
	shaderProgram->link();
	if(!shaderProgram->isLinked())
	{
		cout << "3D Shader Linking Error" << endl;
		cout << "" << shaderProgram->log() << endl << endl;
	}
	shaderProgram->bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

	mShaders[name] = shaderProgram;
}

void ResourceManager::loadTexture(const std::string& name, bool fullPath) {
  Texture* texture = new Texture();

  if (fullPath)
    texture->loadFromFile(name, TEXTURE_PIXEL_FORMAT_RGBA);
  else
    texture->loadFromFile("textures/" + name, TEXTURE_PIXEL_FORMAT_RGBA);

  mTextures[name] = texture;
}

void ResourceManager::loadMusic(const std::string& name) {
  sf::Music* music = new sf::Music();
  music->openFromFile("music/" + name);

  mMusics[name] = music;
}

void ResourceManager::loadSoundBuffer(const std::string& name) {
  sf::SoundBuffer* buffer = new sf::SoundBuffer();
  buffer->loadFromFile("sounds/" + name);

  mSoundBuffers[name] = buffer;
}

void ResourceManager::loadConstants() {
  std::ifstream stream("constants.txt");
  if (!stream.is_open()) {
    std::cout << "Could not open constants file. Exiting game" << std::endl;
    Game::instance().stop();
    return;
  }

  std::string line;
  while (getline(stream,line)) {
    std::istringstream sstream(line);
    std::string command, label;

    if (line.find("int ") != std::string::npos) {
      int value;
      sstream >> command >> label >> value;
      mConstantsInt[label] = value;
      //std::cout << "Read int " << label << " " << value << std::endl;
    } else if (line.find("float ") != std::string::npos) {
      float value;
      sstream >> command >> label >> value;
      mConstantsFloat[label] = value;
      //std::cout << "Read float " << label << " " << value << std::endl;   
    }
  }
}

Gui* ResourceManager::layout(const std::string& name) {
  return GuiReader::loadFromFile(name);
}