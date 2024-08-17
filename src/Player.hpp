#include "Skyrmion/InputHandler.h"

#include "MovableBox.hpp"

std::vector<std::string> miscLayout = {
	"/controls/zoom_in", "/controls/zoom_out", "/controls/reset_room"
};

class Player : public GravityNode {
	DirectionHandler moveInput;

	InputHandler miscInput;
	float zoomLevel = 2;

	Node *section = NULL;
	sf::Vector2f enterPoint = sf::Vector2f(0,0);

	Node *camera = NULL;

public:

	Player(Indexer _collision) : GravityNode(_collision, PLAYER, sf::Vector2i(32, 64)),
		moveInput("/movement", INPUT, this), miscInput(miscLayout, INPUT, this) {

		camera = new Node(INPUT, sf::Vector2i(450, 250), true, this);

		Player *_player = this;
		miscInput.pressedFunc = [_player](int i) {
			if(i < 2) {
				_player->zoomLevel = std::clamp(2.0, _player->zoomLevel + 0.5 * (i ? 1 : -1), 10.0);
				//_player->setScale(sf::Vector2f(_player->zoomLevel, _player->zoomLevel));
				UpdateList::setCamera(_player->camera, sf::Vector2f(450, 250) * _player->zoomLevel);
			} else if(i == 2) {
				_player->setPosition(_player->enterPoint);
				UpdateList::sendSignal(BOX, RESET_SECTION, _player->section);
			}
		};
		UpdateList::setCamera(_player->camera, sf::Vector2f(450, 250) * 2.0f);

		collideWith(BOX);
		collideWith(SECTION);
		collideWith(CAMERA_SECTION);
	}

	void update(double time) {
		sf::Vector2f velocity = gravityVelocity(moveInput.getDirection() * 160.0f, time);
		setPosition(getPosition() + velocity);
	}

	void collide(Node *object) {
		if(object->getLayer() == SECTION || object->getLayer() == CAMERA_SECTION) {
			if(section != object) {
				section = object;
				enterPoint = getPosition();
				//std::cout << "Entering room\n";
				if(object->getLayer() == CAMERA_SECTION) {
					camera->setParent(object);
				} else
					camera->setParent(this);
			}
		} else
			colliding.push_back(object);
	}
};