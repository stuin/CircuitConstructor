#include "Skyrmion/TileMap.hpp"

class GravityNode : public Node {
	float jumpTime = 0;
	float verticalSpeed = 0;
	float horizontalSpeed = 0;

	bool tempPlatforms = false;

public:
	Indexer collision;
	bool blocked = false;
	std::vector<GravityNode *> colliding;
	GridSection *section = NULL;

	Indexer frictionMap;
	float frictionValue = 1;
	float weight = 0;
	float pushWeight = 0;

	int jumpPower = 288;
	int slideSpeed = 100;

	GravityNode(Indexer _collision, Indexer _friction, Layer layer, sf::Vector2i size) :
	Node(layer, size), collision(_collision), frictionMap(_friction) {

	}

	sf::Vector2f gravityVelocity(sf::Vector2f input, double time) {
		bool jumpInput = input.y < 0;
		sf::Vector2f velocity = sf::Vector2f(input.x * time, 0);
		sf::Vector2f pos = getPosition();
		sf::Vector2f foot = pos;
		foot.y += getSize().y / 2 + 4;
		tempPlatforms = section != NULL && section->trigger;

		//Friction
		float friction = (frictionMap.getTile(foot) / 100.0) * frictionValue;
		friction = std::clamp(0.0f, friction, 1.0f);
		if(friction < 1 && collision.getTile(foot) != EMPTY) {
			velocity.x *= friction;
			velocity.x += horizontalSpeed * (1.0 - friction/2) * time;

			if(std::abs(velocity.x) < slideSpeed * time) {
				if(collision.getTile(foot) == SLOPELEFT)
					velocity.x += slideSpeed * (1 - friction) * weight * time;
				else if(collision.getTile(foot) == SLOPERIGHT)
					velocity.x -= slideSpeed * (1 - friction) * weight * time;
			}
		}

		//Check for wall
		sf::Vector2f collisionOffset = velocity + sf::Vector2f(
			velocity.x / std::abs(velocity.x) * getSize().x / 2, getSize().y / 4);
		if(verticalSpeed != 0 || foot.y - 8 < collision.snapPosition(foot).y) {
			if(collision.getTile(pos + collisionOffset) == FULL)
				velocity.x = 0;
			else if(velocity.x > 0 && collision.getTile(pos + collisionOffset) == SLOPELEFT)
				velocity.x = 0;
			else if(velocity.x < 0 && collision.getTile(pos + collisionOffset) == SLOPERIGHT)
				velocity.x = 0;
		}

		//Falling and jumping
		foot += velocity;
		sf::Vector2f footL = foot - sf::Vector2f(getSize().x / 4, 0);
		sf::Vector2f footR = foot + sf::Vector2f(getSize().x / 4, 0);
		if((collision.getTile(footL) == EMPTY || (collision.getTile(footL) == TEMPPLATFORM && !tempPlatforms))
			&& (collision.getTile(footR) == EMPTY || (collision.getTile(footR) == TEMPPLATFORM && !tempPlatforms))) {

			if(jumpTime < 0.2 && jumpInput)
				verticalSpeed -= 2;
			else
				verticalSpeed += 64;
			verticalSpeed = std::min(verticalSpeed, 800.0f);
			velocity.y += verticalSpeed * time;

			//Ceiling check
			if(collision.getTile(pos + velocity) != EMPTY && collision.getTile(pos + velocity) != PLATFORM
				&& collision.getTile(pos + velocity) != TEMPPLATFORM) {
				verticalSpeed = 0;
				velocity.y = 0;
				jumpTime += 0.2;
			}

			foot.y += verticalSpeed * time;
			jumpTime += time;
		} else if(jumpInput && jumpTime == 0) {
			//Start jump
			verticalSpeed = -jumpPower;
			velocity.y += verticalSpeed * time;
		}

		//Snap to ground
		if(collision.getTile(foot) != EMPTY && !(jumpInput && jumpTime < 0.2)
			&& (collision.getTile(foot) != TEMPPLATFORM || tempPlatforms)) {
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
				velocity.y -= pos.x - ground.x - collision.getScale().x;
			velocity.y = std::min(std::max(2.0f, verticalSpeed * (float)time), velocity.y);

			if(velocity.y == 0) {
				verticalSpeed = 0;
				jumpTime = 0;
			}
		}

		for(GravityNode *other : colliding) {
			if(getPosition().y + getSize().y/2 >= other->getPosition().y - other->getSize().y/2) {
				if(jumpInput && jumpTime == time) {
					jumpTime = 0;
					verticalSpeed = -jumpPower;
					velocity.y += verticalSpeed * time;
				} else if(velocity.y > 0) {
					velocity.y = (other->getPosition().y - other->getSize().y/2) - (pos.y + getSize().y/2)+2;
					//velocity.y = 0;
					jumpTime = 0;
				}
			} else if(velocity.x != 0 && (other->getPosition().x - getPosition().x) * velocity.x > 0) {
				if(other->blocked)
					velocity.x = 0;
				else if(other->weight != 0)
					velocity.x -= velocity.x * other->weight;
			}
		}
		pushWeight = weight;
		colliding.clear();

		horizontalSpeed = velocity.x / time;
		blocked = std::abs(input.x) > 0.1 && std::abs(velocity.x) < 0.1;
		return velocity;
	}
};