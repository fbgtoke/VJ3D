#include "ModelAnimated.h"

void ModelAnimated::update(int deltaTime) {
  Model::update(deltaTime);

  mAnimation.update(deltaTime);
}

void ModelAnimated::beforeRender() {
  Model::beforeRender();
  
  const AnimationFrame* frame = mAnimation.getCurrentFrame();

  if (frame != nullptr) {
    Mesh* mesh = frame->mesh;
    setMesh(mesh);
  }
}

glm::mat4 ModelAnimated::getTransform() const {
  const AnimationFrame* frame = mAnimation.getCurrentFrame();

  if (frame != nullptr)
    return frame->transform * Model::getTransform();

  return Model::getTransform();
}

Animation3D& ModelAnimated::getAnimation() { return mAnimation; }