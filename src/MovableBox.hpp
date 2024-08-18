#include "GravityNode.hpp"

class MovableBox : public GravityNode {
	sf::Vector2f pushDirection = sf::Vector2f(0,0);
	sf::Vector2f startPosition;

public:
	float weight = 1;

	MovableBox(Indexer _collision, Indexer _friction, uint c, sf::Vector2f _startPosition, sf::Texture *blockTexture) :
	GravityNode(_collision, _friction, BOX, sf::Vector2i(16, 16)), startPosition(_startPosition) {
		setPosition(startPosition);
		setScale(sf::Vector2f(1.5, 1.5));
		setTexture(blocksTexture);

		if(c == 'i' || c == 'i' + SNOW_OFFSET) {
			b->setTextureRect(sf::IntRect(0, 16, 16, 16));
			b->frictionValue = 0.5;
		} else {
			b->setTextureRect(sf::IntRect(
				(c == 'w' || c == 'w' + SNOW_OFFSET) ? 48 : 32, 0, 16, 16));
		}

		collideWith(PLAYER);
		collideWith(BOX);
		collideWith(SECTION);
		UpdateList::addNode(this);
	}

	void update(double time) {
		sf::Vector2f velocity = gravityVelocity(pushDirection * 16.0f / weight, time);
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
		if(object->getLayer() == SECTION) {
			section = (GridSection *) object;
			collideWith(SECTION, false);
		} else {
			float x = getPosition().x - object->getPosition().x;
			if(std::abs(x) > getSize().x)
				pushDirection.x += x;
			colliding.push_back(object);
		}
	}

	void recieveSignal(int id, Node *sender) {
		if(id == RESET_SECTION && sender == section)
			setPosition(startPosition);
	}
};