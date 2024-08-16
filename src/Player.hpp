#include "Skyrmion/TileMap.hpp"

class Player : public Node {
	DirectionHandler input;
	Indexer collisionMap;

public:

	Player(Indexer _collisionMap) : Node(PLAYER),
		input("/movement", INPUT, this), collisionMap(_collisionMap) {
	}

	void update(double time) {
		sf::Vector2f prevPosition = getPosition();
		sf::Vector2f target = move(input.getDirection(), &collisionMap, time * 32);

	}

	void collide(Node *object) {

	}
};