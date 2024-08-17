#include "MovableBox.hpp"

std::vector<std::string> zoomLayout = {
	"/zoom/in", "/zoom/out"
};

class Player : public GravityNode {
	DirectionHandler moveInput;

	InputHandler zoomInput;
	float zoomLevel = 1;

public:

	Player(Indexer _collision) : GravityNode(_collision, PLAYER),
		moveInput("/movement", INPUT, this), zoomInput(zoomLayout, INPUT, this) {

		Player *_player = this;
		zoomInput.pressedFunc = [_player](int i) {
			_player->zoomLevel = std::max(1.0, _player->zoomLevel + 0.5 * (i ? 1 : -1));
			//_player->setScale(sf::Vector2f(_player->zoomLevel, _player->zoomLevel));
			UpdateList::setCamera(_player, sf::Vector2f(450, 250) * _player->zoomLevel);
		};
		UpdateList::setCamera(_player, sf::Vector2f(450, 250));

		collideWith(BOX);
	}

	void update(double time) {
		sf::Vector2f velocity = gravityVelocity(moveInput.getDirection() * 80.0f, time);
		setPosition(getPosition() + velocity);
	}

	void collide(Node *object) {
		colliding.push_back(object);
	}
};