#include "ModelAnimated.h"

void ModelAnimated::update(int deltaTime) {
  Model::update(deltaTime);

  mAnimation.update(deltaTime);
}

void ModelAnimated::render() {
  const AnimationFrame* frame = mAnimation.getCurrentFrame();

  if (frame != nullptr) {
    Mesh* mesh = frame->mesh;
    if (mesh != nullptr) {
      setMesh(mesh);
      Model::render();
    }
  }
}

glm::mat4 ModelAnimated::getTransform() const {
  const AnimationFrame* frame = mAnimation.getCurrentFrame();

  if (frame != nullptr)
    return frame->transform * Model::getTransform();

  return Model::getTransform();
}

Animation3D& ModelAnimated::getAnimation() { return mAnimation; }