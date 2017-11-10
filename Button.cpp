#include "Button.h"
#include "Game.h"

Button::Button(
	const glm::vec2 &quadSize,
	const glm::vec2 &sizeInSpritesheet,
	Texture* spritesheet,
	ShaderProgram* program,
	const glm::vec2& position,
	std::function<void()> onClick)
	: Sprite(quadSize, sizeInSpritesheet, spritesheet, program),
	mOnClick(onClick)
{
	setNumberAnimations(1);
	setAnimationSpeed(0, 0);
	addKeyframe(0, glm::vec2(0.0f, 0.0f));
	setPosition(position);
}

void Button::update(int deltaTime) {
	Sprite::update(deltaTime);

	if (Game::instance().getMouseButtonPressed())
		if (inBounds(Game::instance().getMousePosition()))
			click();
}

void Button::setOnClick(std::function<void()> onClick) { mOnClick = onClick; }

void Button::click() { mOnClick(); }

bool Button::inBounds(const glm::ivec2& pos) {
	return
		pos.x >= position.x && pos.x <= position.x + mQuadSize.x &&
		pos.y >= position.y && pos.y <= position.y + mQuadSize.y;
}