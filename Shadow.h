#ifndef _SHADOW_INCLUDE
#define _SHADOW_INCLUDE

#include "Model.h"

class Shadow : public Model {
public:
  Shadow(const Model* model);
  ~Shadow() override;

  void init() override;
  void update(int deltaTime) override;
  void render() override;

  void unbind();

private:
  const Model* mModel;
};

#endif // _SHADOW_INCLUDE