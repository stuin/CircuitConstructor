#include "Skyrmion/tiling/TileMap.hpp"
#include "Skyrmion/input/MovementFunctions.hpp"

class MovableBox : public Node, public PhysicsObject {
	Indexer *collisionOn;
	Indexer *collisionOff;
	sf::Vector2f startPosition;

	Indexer *frictionMap;
	float frictionValue = 1;

	PersonalPhysicsStats *physics = new PersonalPhysicsStats();
	GlobalPhysicsStats *globalPhysics = new GlobalPhysicsStats();
	std::vector<PersonalPhysicsStats *> colliding;

	GridSection *section = NULL;
	GridSection *mainSection = NULL;

public:
	MovableBox(Indexer *_collisionOn, Indexer *_collisionOff, Indexer *_friction, uint c, sf::Vector2f _startPosition, sf::Texture *blockTexture) :
	Node(BOX, sf::Vector2i(16, 16)), collisionOn(_collisionOn), collisionOff(_collisionOff), startPosition(_startPosition),
	frictionMap(_friction) {

		setPosition(startPosition);
		setScale(sf::Vector2f(3, 3));
		setTexture(*blockTexture);
		physics->weight = 0.2;
		frictionValue = 0.85;

		switch(c) {
		case 'i': case 'i'+SNOW_OFFSET:
			setTextureRect(sf::IntRect(0, 16, 16, 16));
			frictionValue = 0.2;
			physics->weight = 0.05;
			break;
		case 'w': case 'w'+SNOW_OFFSET:
			setTextureRect(sf::IntRect(48, 0, 16, 16));
			break;
		case 'g': case 'g'+SNOW_OFFSET:
			setTextureRect(sf::IntRect(32, 0, 16, 16));
			physics->weight = 0.5;
			break;
		case 'm': case 'm'+SNOW_OFFSET:
			setTextureRect(sf::IntRect(16, 16, 16, 16));
			physics->weight = 0.5;
			setScale(sf::Vector2f(5, 5));
			break;
		}

		collideWith(PLAYER);
		collideWith(BOX);
		collideWith(SECTION);
		UpdateList::addNode(this);
	}

	void update(double time) {
		bool tempPlatforms = section != NULL && (section->trigger != section->invertTrigger);
		Indexer *collision = tempPlatforms ? collisionOn : collisionOff;

		//std::cout << pushDirection << "\n";
		sf::Vector2f velocity = physics->pushDirection * (float)time;
		velocity = PlatformFrictionMovement(getPosition(), velocity, getSize(), time,
			physics->previous, collision, frictionMap, frictionValue, globalPhysics);
		velocity = PlatformGravityMovement(getPosition(), velocity, getSize(), time, false,
			collision, globalPhysics, physics, colliding);
		setPosition(getPosition() + velocity);
		colliding.clear();

		if(!Settings::getBool("/debug_block_rotation")) {
			int floor = collision->getTile(getPosition() + sf::Vector2f(0, getSize().x/2+4));
			if(floor == SLOPE_UPLEFT)
				setRotation(-45);
			else if(floor == SLOPE_UPRIGHT)
				setRotation(45);
			else
				setRotation(0);
		}
	}

	void collide(Node *other) {
		if(other->getLayer() == SECTION) {
			if(mainSection == NULL)
				mainSection = (GridSection *) other;
			section = (GridSection *) other;
		} else if(other->getLayer() == BOX || other->getLayer() == PLAYER)
			colliding.push_back(dynamic_cast<PhysicsObject*>(other)->getPhysics());
	}

	void recieveSignal(int id, Node *sender) {
		if(id == RESET_SECTION && (sender == section || sender == mainSection))
			setPosition(startPosition);
		else if(id == RESET_GAME)
			setPosition(startPosition);
	}

	PersonalPhysicsStats *getPhysics() override {
		return physics;
	}
};