#ifndef _DEPTH_BUFFER_INCLUDE
#define _DEPTH_BUFFER_INCLUDE

#include "utils.h"
#include "Texture.h"

class DepthBuffer {
public:
  DepthBuffer();
  ~DepthBuffer();

  void init();
  void use();

  Texture* getTexture();

private:
  GLuint mFBO, mDepthbuffer;
  Texture* mTexture;
};

#endif // _DEPTH_BUFFER_INCLUDE