#include "DepthBuffer.h"
#include "Game.h"

DepthBuffer::DepthBuffer() {}

DepthBuffer::~DepthBuffer() {
  glDeleteFramebuffers(1, &mFBO);
}

void DepthBuffer::init() {
  glGenFramebuffers(1, &mFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

  glGenTextures(1, &mTexture);
  glBindTexture(GL_TEXTURE_2D, mTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mTexture, 0);
  glDrawBuffer(GL_NONE);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "Could not create depth buffer" << std::endl;
    Game::instance().stop();
  }
}

void DepthBuffer::use() {
  glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLuint DepthBuffer::getTexture() const { return mTexture; }