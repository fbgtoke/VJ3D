#ifndef _FRAME_BUFFER_INCLUDE
#define _FRAME_BUFFER_INCLUDE

#include "utils.h"
#include "Texture.h"

class FrameBuffer {
public:
  FrameBuffer();
  virtual ~FrameBuffer();

  void init();
  void use();
  Texture* getTexture();

private:
  GLuint mFBO, mDepthbuffer;
  Texture* mTexture;
};

#endif // _FRAME_BUFFER_INCLUDE
