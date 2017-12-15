#include "ModelAnimated.h"

void ModelAnimated::update(int deltaTime) {
  Model::update(deltaTime);

  mAnimation.update(deltaTime);
}

void ModelAnimated::render() {
  Mesh* mesh = mAnimation.getCurrentFrame();

  if (mesh != nullptr) {
    setMesh(mesh);
    Model::render();
  }
}

Animation3D& ModelAnimated::getAnimation() { return mAnimation; }