#include "GravityNode.hpp"

class MovableBox : public GravityNode {
	sf::Vector2f pushDirection = sf::Vector2f(0,0);

public:
	MovableBox(Indexer _collision, sf::Vector2i size) : GravityNode(_collision, BOX, size) {
		collideWith(PLAYER);
		collideWith(BOX);
	}

	void update(double time) {
		sf::Vector2f velocity = gravityVelocity(pushDirection * 8.0f, time);
		setPosition(getPosition() + velocity);

		int floor = collision.getTile(getPosition() + sf::Vector2f(0, getSize().x/2));
		if(floor == SLOPELEFT)
			setRotation(-40);
		else if(floor == SLOPERIGHT)
			setRotation(40);
		else
			setRotation(0);

		pushDirection = sf::Vector2f(0,0);
	}

	void collide(Node *object) {
		float x = getPosition().x - object->getPosition().x;
		if(std::abs(x) > 10)
			pushDirection.x += x;
		colliding.push_back(object);
	}
};