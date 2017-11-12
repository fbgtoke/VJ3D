#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Sprite.h"


Sprite *Sprite::createSprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program)
{
	Sprite *quad = new Sprite(quadSize, sizeInSpritesheet, spritesheet, program);

	return quad;
}


Sprite::Sprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program)
{
	float vertices[24] = {0.f, 0.f, 0.f, 0.f, 
		quadSize.x, 0.f, sizeInSpritesheet.x, 0.f, 
		quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y, 
		0.f, 0.f, 0.f, 0.f, 
		quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y, 
		0.f, quadSize.y, 0.f, sizeInSpritesheet.y};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocation = program->bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program->bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
	texture = spritesheet;
	shaderProgram = program;
	currentAnimation = -1;
	position = glm::vec2(0.f);

	mRepeat = true;

	mQuadSize = quadSize;
	mSizeInSpritesheet = sizeInSpritesheet;
}

void Sprite::update(int deltaTime)
{
	if(currentAnimation >= 0 && animations[currentAnimation].millisecsPerKeyframe != 0)
	{
		timeAnimation += deltaTime;
		while(timeAnimation > abs(animations[currentAnimation].millisecsPerKeyframe))
		{
			timeAnimation -= abs(animations[currentAnimation].millisecsPerKeyframe);

			if (animations[currentAnimation].millisecsPerKeyframe >= 0) {
				if (currentKeyframe < animations[currentAnimation].keyframeDispl.size() - 1 || mRepeat)
					currentKeyframe = (currentKeyframe + 1) % animations[currentAnimation].keyframeDispl.size();
			} else {
				if (currentKeyframe > 0 || mRepeat)
					currentKeyframe = (animations[currentAnimation].keyframeDispl.size() + currentKeyframe - 1) % animations[currentAnimation].keyframeDispl.size();
			}
		}
		texCoordDispl = animations[currentAnimation].keyframeDispl[currentKeyframe];
	}
}

void Sprite::render() const {
	glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.f));
	shaderProgram->setUniformMatrix4f("modelview", modelview);
	shaderProgram->setUniform2f("texCoordDispl", texCoordDispl.x, texCoordDispl.y);
	
	texture->use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glDisable(GL_TEXTURE_2D);
}

void Sprite::free() {
	glDeleteBuffers(1, &vbo);
}

void Sprite::setNumberAnimations(int nAnimations) {
	animations.clear();
	animations.resize(nAnimations);
}

void Sprite::setAnimationSpeed(int animId, int keyframesPerSec) {
	if(animId < int(animations.size())) {
		if (keyframesPerSec != 0)
			animations[animId].millisecsPerKeyframe = 1000.f / keyframesPerSec;
		else
			animations[animId].millisecsPerKeyframe = 0;
	}
}

void Sprite::addKeyframe(int animId, const glm::vec2 &displacement) {
	if(animId < int(animations.size()))
		animations[animId].keyframeDispl.push_back(displacement);
}

void Sprite::changeAnimation(int animId) {
	if(animId < int(animations.size()))
	{
		currentAnimation = animId;
		currentKeyframe = 0;
		timeAnimation = 0.f;
		texCoordDispl = animations[animId].keyframeDispl[0];
	}
}

int Sprite::animation() const
{
	return currentAnimation;
}

void Sprite::setPosition(const glm::vec2 &pos)
{
	position = pos;
}

glm::vec2 Sprite::getQuadSize() const {
	return mQuadSize;
}

glm::vec2 Sprite::getPosition() const {
	return position;
}

void Sprite::move(const glm::vec2& pos) {
	position += pos;
}

void Sprite::setRepeat(bool set) {
	mRepeat = set;
}

void Sprite::setKeyFrame(int animId, int keyFrameId) {
	if (animId < int(animations.size()))
		if (keyFrameId < int (animations[animId].keyframeDispl.size()))
			currentKeyframe = keyFrameId;
}

unsigned int Sprite::getKeyFrame() const {
	return currentKeyframe;
}

Sprite* Sprite::clone() const {
	Sprite* sprite = createSprite(mQuadSize, mSizeInSpritesheet, texture, shaderProgram);
	sprite->position = position;
	sprite->currentAnimation = currentAnimation;
	sprite->currentKeyframe = currentKeyframe;
	sprite->timeAnimation = timeAnimation;
	sprite->texCoordDispl = texCoordDispl;
	sprite->animations = animations;
	sprite->mRepeat = mRepeat;

	return sprite;
}