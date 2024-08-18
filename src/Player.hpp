#include "Skyrmion/InputHandler.h"

#include "MovableBox.hpp"

std::vector<std::string> miscLayout = {
	"/controls/zoom_in", "/controls/zoom_out", "/controls/reset_room"
};

class Player : public GravityNode {
	DirectionHandler moveInput;
	InputHandler miscInput;

	sf::Vector2f enterPoint = sf::Vector2f(0,0);
	float zoomLevel = 1.5;
	float zoomTarget = 1.5;

	Node *camera = NULL;

public:

	Player(Indexer _collision, Indexer _friction) : GravityNode(_collision, _friction, PLAYER, sf::Vector2i(16, 20)),
		moveInput("/movement", INPUT, this), miscInput(miscLayout, INPUT, this) {

		camera = new Node(INPUT, sf::Vector2i(450, 250), true, this);

		Player *_player = this;
		miscInput.pressedFunc = [_player](int i) {
			if(i < 2)
				_player->zoomTarget = std::clamp(1.0, _player->zoomTarget + 0.2 * (i ? 1 : -1), 10.0);
			else if(i == 2) {
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

		//Slide camera
		if(camera->getPosition() != sf::Vector2f(0,0)) {
			sf::Vector2f target = camera->getPosition() * -1.0f;
			sf::Vector2f target2 = vectorLength(target, 200 * time);
			if(std::abs(target.x) > std::abs(target2.x) && std::abs(target.x) > std::abs(target2.x))
				target = target2;
			camera->setPosition(camera->getPosition() + target);
		}

		//Update zoom level
		if(zoomLevel != zoomTarget) {
			zoomTarget = std::clamp(-10.0f, zoomTarget, 10.0f);
			zoomLevel += std::clamp(-0.02f, zoomTarget - zoomLevel, 0.02f);
			UpdateList::setCamera(camera, sf::Vector2f(450, 250) * zoomLevel);
		}
	}

	void collide(Node *object) {
		if(object->getLayer() == SECTION) {
			if(section != object) {
				section = (GridSection *)object;
				enterPoint = getPosition();
				//std::cout << "Entering room\n";

				//Adjust camera
				sf::Vector2f cameraPosition = camera->getGPosition();
				if(section->grabCamera)
					camera->setParent(object);
				else
					camera->setParent(this);
				camera->setGPosition(cameraPosition);
				if(section->zoomLevel != 0)
					zoomTarget = section->zoomLevel;
			}
		} else
			colliding.push_back(object);
	}
};