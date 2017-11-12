#include "Button.h"
#include "Game.h"

Button::Button(
	const glm::vec2 &quadSize,
	Texture* spritesheet,
	ShaderProgram* program,
	const glm::vec2& position,
	std::function<void()> onClick)
	: Sprite(quadSize, glm::vec2(1.0f, 0.5f), spritesheet, program),
	mOnClick(onClick)
{
	setNumberAnimations(2);
	
	setAnimationSpeed(0, 0);
	addKeyframe(0, glm::vec2(0.0f, 0.0f));

	setAnimationSpeed(1, 0);
	addKeyframe(1, glm::vec2(0.0f, 0.5f));
	
	setPosition(position);

	mState = IDLE;
}

void Button::update(int deltaTime) {
	Sprite::update(deltaTime);

	switch (mState) {
	case IDLE:
		if (inBounds(Game::instance().getMousePosition()))
			changeState(HOVER);
		break;
	case HOVER:
		if (!inBounds(Game::instance().getMousePosition()))
			changeState(IDLE);
		else if (Game::instance().getMouseButtonPressed())
			changeState(ACTIVE);
		break;
	case ACTIVE:
		if (Game::instance().getMouseButtonReleased())
			if (inBounds(Game::instance().getMousePosition()))
				changeState(HOVER);
			else
				changeState(IDLE);
		break;
	default:
		break;
	}
}

void Button::setOnClick(std::function<void()> onClick) { mOnClick = onClick; }

void Button::click() { mOnClick(); }

void Button::changeState(ButtonState state) {
	mState = state;

	if (state == IDLE) {
		changeAnimation(0);
	} else if (state == HOVER) {
		changeAnimation(1);
	} else {
		changeAnimation(1);
		click();
	}
}

bool Button::inBounds(const glm::ivec2& pos) {
	return
		pos.x >= position.x && pos.x <= position.x + mQuadSize.x &&
		pos.y >= position.y && pos.y <= position.y + mQuadSize.y;
}