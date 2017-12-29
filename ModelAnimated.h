#ifndef _MODEL_ANIMATED_INCLUDE
#define _MODEL_ANIMATED_INCLUDE

#include "Model.h"
#include "Animation3D.h"

class ModelAnimated : public Model {
public:
  void update(int deltaTime) override;

  glm::mat4 getTransform() const override;
  Animation3D& getAnimation();

protected:
  void beforeRender() override;
  
  Animation3D mAnimation;
};

#endif // _MODEL_ANIMATED_INCLUDE