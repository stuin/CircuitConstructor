#include "GravityNode.hpp"

class MovableBox : public GravityNode {
	sf::Vector2f pushDirection = sf::Vector2f(0,0);
	sf::Vector2f startPosition;

public:
	MovableBox(Indexer _collision, sf::Vector2f _startPosition, sf::Vector2i size) :
	GravityNode(_collision, BOX, size), startPosition(_startPosition) {
		setPosition(startPosition);

		collideWith(PLAYER);
		collideWith(BOX);
	}

	void update(double time) {
		sf::Vector2f velocity = gravityVelocity(pushDirection * 16.0f, time);
		setPosition(getPosition() + velocity);

		int floor = collision.getTile(getPosition() + sf::Vector2f(0, getSize().x/2+4));
		if(floor == SLOPELEFT)
			setRotation(-45);
		else if(floor == SLOPERIGHT)
			setRotation(45);
		else
			setRotation(0);

		pushDirection = sf::Vector2f(0,0);
	}

	void collide(Node *object) {
		float x = getPosition().x - object->getPosition().x;
		if(std::abs(x) > 20)
			pushDirection.x += x;
		colliding.push_back(object);
	}
};