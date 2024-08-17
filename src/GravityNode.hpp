#include "Skyrmion/TileMap.hpp"

class GravityNode : public Node {
	float jumpTime = 0;
	float verticalSpeed = 0;

public:
	Indexer collision;
	bool blocked = false;
	std::vector<Node *> colliding;

	GravityNode(Indexer _collision, Layer layer, sf::Vector2i size) :
	Node(layer, size), collision(_collision) {

	}

	sf::Vector2f gravityVelocity(sf::Vector2f input, double time) {
		bool jumpInput = input.y < 0;
		sf::Vector2f velocity = sf::Vector2f(input.x * time, 0);
		input.x /= std::abs(input.x);
		sf::Vector2f collisionOffset = velocity + sf::Vector2f(input.x * getSize().x / 2, getSize().y / 4);

		sf::Vector2f pos = getPosition();
		sf::Vector2f foot = pos;
		foot.y += getSize().y / 2 + 4;

		//Check for wall
		if(verticalSpeed != 0 || foot.y - 8 < collision.snapPosition(foot).y) {
			if(collision.getTile(pos + collisionOffset) == FULL)
				velocity.x = 0;
			else if(velocity.x > 0 && collision.getTile(pos + collisionOffset) == SLOPELEFT)
				velocity.x = 0;
			else if(velocity.x < 0 && collision.getTile(pos + collisionOffset) == SLOPERIGHT)
				velocity.x = 0;
		}

		//Up and down
		foot += velocity;
		if(collision.getTile(foot) == EMPTY) {
			if(jumpTime < 0.2 && jumpInput)
				verticalSpeed -= 12;
			else
				verticalSpeed += 32;
			verticalSpeed = std::min(verticalSpeed, 400.0f);
			velocity.y += verticalSpeed * time;

			//Ceiling check
			if(collision.getTile(pos + velocity) != EMPTY && collision.getTile(pos + velocity) != PLATFORM) {
				verticalSpeed = 0;
				velocity.y = 0;
				jumpTime += 0.2;
			}

			foot.y += verticalSpeed * time;
			jumpTime += time;
		} else if(jumpInput && jumpTime == 0) {
			//Start jump
			verticalSpeed = -176;
			velocity.y += verticalSpeed * time;
		}

		if(collision.getTile(foot) != EMPTY && !(jumpInput && jumpTime < 0.2)) {
			sf::Vector2f ground = collision.snapPosition(foot);
			sf::Vector2f foot2 = foot - sf::Vector2f(0, 8);

			//Allow for upwards slope
			if(collision.getTile(foot) != EMPTY &&
				(collision.getTile(foot2) == SLOPELEFT || collision.getTile(foot2) == SLOPERIGHT)) {
				foot = foot2;
				ground = collision.snapPosition(foot);
			}
			velocity.y = ground.y - pos.y - getSize().y / 2;

			if(collision.getTile(foot) == SLOPELEFT)
				velocity.y -= ground.x - pos.x;
			else if(collision.getTile(foot) == SLOPERIGHT)
				velocity.y -= pos.x - ground.x - collision.getSize().x;
			velocity.y = std::min(std::max(2.0f, verticalSpeed * (float)time), velocity.y);

			if(velocity.y == 0) {
				verticalSpeed = 0;
				jumpTime = 0;
			}
		}

		for(Node *other : colliding) {
			if(other->getPosition().y > getPosition().y + getSize().y / 2) {
				if(jumpInput && jumpTime == time) {
					jumpTime = 0;
					verticalSpeed = -176;
					velocity.y += verticalSpeed * time;
				} else if(velocity.y > 0) {
					velocity.y = 0;
					jumpTime = 0;
				}
			}
			if(velocity.x != 0 && ((GravityNode*)other)->blocked &&
				(other->getPosition().x - getPosition().x) * velocity.x > 0)
				velocity.x = 0;
		}
		colliding.clear();

		blocked = std::abs(input.x) > 0.1 && std::abs(velocity.x) < 0.1;
		return velocity;
	}
};