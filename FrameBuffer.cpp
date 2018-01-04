#include "FrameBuffer.h"
#include "Game.h"

FrameBuffer::FrameBuffer()
  : mTexture(nullptr) {}

FrameBuffer::~FrameBuffer() {
  glDeleteFramebuffers(1, &mFBO);

  if (mTexture != nullptr) delete mTexture;
}

void FrameBuffer::init() {
  mFBO = 0;
  glGenFramebuffers(1, &mFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

  mTexture = new Texture();
  mTexture->createEmptyTexture(SCREEN_WIDTH, SCREEN_HEIGHT,GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
  mTexture->setMinFilter(GL_NEAREST);
  mTexture->setMagFilter(GL_NEAREST);
  GLuint texId = mTexture->getTexId();

  glGenRenderbuffers(1, &mDepthbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, mDepthbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthbuffer);

  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texId, 0);

  GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (result != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "Could not generate frame buffer" << std::endl;
    return;
  }
}

void FrameBuffer::use() {
  glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
  glDrawBuffer(GL_COLOR_ATTACHMENT0);
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Texture* FrameBuffer::getTexture() { return mTexture; }
