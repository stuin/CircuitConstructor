#include "Skyrmion/InputHandler.h"

#include "MovableBox.hpp"

std::vector<std::string> miscLayout = {
	"/controls/zoom_in", "/controls/zoom_out", "/controls/reset_room"
};

class Player : public GravityNode {
	DirectionHandler moveInput;
	InputHandler miscInput;

	float zoomLevel = 1.5;
	sf::Vector2f enterPoint = sf::Vector2f(0,0);
	Node *camera = NULL;

public:

	Player(Indexer _collision, Indexer _friction) : GravityNode(_collision, _friction, PLAYER, sf::Vector2i(16, 20)),
		moveInput("/movement", INPUT, this), miscInput(miscLayout, INPUT, this) {

		camera = new Node(INPUT, sf::Vector2i(450, 250), true, this);

		Player *_player = this;
		miscInput.pressedFunc = [_player](int i) {
			if(i < 2) {
				_player->zoomLevel = std::clamp(1.0, _player->zoomLevel + 0.5 * (i ? 1 : -1), 10.0);
				//_player->setScale(sf::Vector2f(_player->zoomLevel, _player->zoomLevel));
				UpdateList::setCamera(_player->camera, sf::Vector2f(450, 250) * _player->zoomLevel);
			} else if(i == 2) {
				_player->setPosition(_player->enterPoint);
				UpdateList::sendSignal(BOX, RESET_SECTION, _player->section);
			}
		};
		UpdateList::setCamera(_player->camera, sf::Vector2f(450, 250) * 1.5f);

		setScale(sf::Vector2f(2,2));

		collideWith(BOX);
		collideWith(SECTION);
	}

	void update(double time) {
		sf::Vector2f velocity = gravityVelocity(moveInput.getDirection() * 160.0f, time);
		setPosition(getPosition() + velocity);

		UpdateList::hideLayer(TEMPMAP, section == NULL || !section->trigger);

		if(velocity.x < 0)
			setScale(sf::Vector2f(-2,2));
		else if(velocity.x > 0)
			setScale(sf::Vector2f(2,2));
	}

	void collide(Node *object) {
		if(object->getLayer() == SECTION) {
			if(section != object) {
				section = (GridSection *)object;
				enterPoint = getPosition();
				//std::cout << "Entering room\n";
				if(section->grabCamera) {
					camera->setParent(object);
				} else
					camera->setParent(this);
			}
		} else
			colliding.push_back(object);
	}
};