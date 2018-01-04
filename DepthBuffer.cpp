#include "DepthBuffer.h"
#include "Game.h"

DepthBuffer::DepthBuffer()
  : mTexture(nullptr) {}

DepthBuffer::~DepthBuffer() {
  glDeleteFramebuffers(1, &mFBO);

  if (mTexture != nullptr) delete mTexture;
}

void DepthBuffer::init() {
  glGenFramebuffers(1, &mFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

  mTexture = new Texture();
  mTexture->createEmptyTexture(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    GL_DEPTH_COMPONENT32,
    GL_DEPTH_COMPONENT,
    GL_FLOAT
  );
  mTexture->setWrapS(GL_CLAMP_TO_EDGE);
  mTexture->setWrapT(GL_CLAMP_TO_EDGE);
  mTexture->setMinFilter(GL_NEAREST);
  mTexture->setMagFilter(GL_NEAREST);
  mTexture->setTexUnit(GL_TEXTURE1);
  
  GLuint texId = mTexture->getTexId();
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texId, 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "Could not create depth buffer" << std::endl;
    Game::instance().stop();
  }
}

void DepthBuffer::use() {
  glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
  glDrawBuffer(GL_DEPTH_ATTACHMENT);
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Texture* DepthBuffer::getTexture() { return mTexture; }
