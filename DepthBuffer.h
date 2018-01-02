#ifndef _DEPTH_BUFFER_INCLUDE
#define _DEPTH_BUFFER_INCLUDE

#include "utils.h"

class DepthBuffer {
public:
  DepthBuffer();
  ~DepthBuffer();

  void init();
  void use();

  GLuint getTexture() const;

private:
  GLuint mFBO, mTexture, mDepthbuffer;
};

#endif // _DEPTH_BUFFER_INCLUDE