#ifndef _SHADOW_INCLUDE
#define _SHADOW_INCLUDE

#include "Model.h"

class Shadow : public Model {
public:
  Shadow(Model* model);
  ~Shadow() override;

  void init() override;
  void update(int deltaTime) override;
  void render() override;

private:
  Model* mModel;
};

#endif // _SHADOW_INCLUDE