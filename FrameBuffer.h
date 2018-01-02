#ifndef _FRAME_BUFFER_INCLUDE
#define _FRAME_BUFFER_INCLUDE

#include "utils.h"

class FrameBuffer {
public:
  FrameBuffer();
  virtual ~FrameBuffer();

  void init();
  void use();
  GLuint getTexture() const;

private:
  GLuint mFBO, mTexture, mDepthbuffer;
};

#endif // _FRAME_BUFFER_INCLUDE
