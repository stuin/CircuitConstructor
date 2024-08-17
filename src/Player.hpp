#include "MovableBox.hpp"

std::vector<std::string> zoomLayout = {
	"/zoom/in", "/zoom/out"
};

class Player : public GravityNode {
	DirectionHandler moveInput;

	InputHandler zoomInput;
	float zoomLevel = 2;

public:

	Player(Indexer _collision) : GravityNode(_collision, PLAYER, sf::Vector2i(32, 64)),
		moveInput("/movement", INPUT, this), zoomInput(zoomLayout, INPUT, this) {

		Player *_player = this;
		zoomInput.pressedFunc = [_player](int i) {
			_player->zoomLevel = std::max(2.0, _player->zoomLevel + 0.5 * (i ? 1 : -1));
			//_player->setScale(sf::Vector2f(_player->zoomLevel, _player->zoomLevel));
			UpdateList::setCamera(_player, sf::Vector2f(450, 250) * _player->zoomLevel);
		};
		UpdateList::setCamera(_player, sf::Vector2f(450, 250) * 2.0f);

		collideWith(BOX);
	}

	void update(double time) {
		sf::Vector2f velocity = gravityVelocity(moveInput.getDirection() * 160.0f, time);
		setPosition(getPosition() + velocity);
	}

	void collide(Node *object) {
		colliding.push_back(object);
	}
};