#include "GravityNode.hpp"

class MovableBox : public GravityNode {
	sf::Vector2f pushDirection = sf::Vector2f(0,0);
	sf::Vector2f startPosition;

public:
	MovableBox(Indexer _collision, Indexer _friction, uint c, sf::Vector2f _startPosition, sf::Texture *blockTexture) :
	GravityNode(_collision, _friction, BOX, sf::Vector2i(16, 16)), startPosition(_startPosition) {
		setPosition(startPosition);
		setScale(sf::Vector2f(3, 3));
		setTexture(*blockTexture);
		weight = 0.4;

		switch(c) {
		case 'i': case 'i'+SNOW_OFFSET:
			setTextureRect(sf::IntRect(0, 16, 16, 16));
			frictionValue = 0.5;
			break;
		case 'w': case 'w'+SNOW_OFFSET:
			setTextureRect(sf::IntRect(48, 0, 16, 16));
			frictionValue = 0.9;
			break;
		case 'g': case 'g'+SNOW_OFFSET:
			setTextureRect(sf::IntRect(32, 0, 16, 16));
			weight = 1;
			break;
		case 'm': case 'm'+SNOW_OFFSET:
			setTextureRect(sf::IntRect(16, 16, 16, 16));
			weight = 1;
			setScale(sf::Vector2f(5, 5));
			break;
		}

		collideWith(PLAYER);
		collideWith(BOX);
		collideWith(SECTION);
		UpdateList::addNode(this);
	}

	void update(double time) {
		sf::Vector2f velocity = gravityVelocity(pushDirection * 20.0f / (1+weight), time);
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

	void collide(Node *other) {
		if(other->getLayer() == SECTION) {
			section = (GridSection *) other;
			collideWith(SECTION, false);
		} else {
			float x = getPosition().x - other->getPosition().x;
			if(getPosition().y - getSize().y/2 < other->getPosition().y + other->getSize().y/3) {
				pushDirection.x += x;
				//pushWeight += ob->weight;
			}
			colliding.push_back((GravityNode*)other);
		}
	}

	void recieveSignal(int id, Node *sender) {
		if(id == RESET_SECTION && sender == section)
			setPosition(startPosition);
	}
};