#include "ResourceManager.h"

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

Texture* ResourceManager::texture(const std::string& name) {
	if (mTextures.count(name) == 0)
		loadTexture(name);

	return mTextures[name];
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
  mesh->initVAO(shader("simple"));

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

void ResourceManager::loadTexture(const std::string& name) {
  Texture* texture = new Texture();
  texture->loadFromFile("textures/" + name, TEXTURE_PIXEL_FORMAT_RGBA);

  mTextures[name] = texture;
}