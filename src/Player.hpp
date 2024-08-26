#include "MovableBox.hpp"

std::vector<std::string> miscLayout = {
	"/controls/zoom_in", "/controls/zoom_out", "/controls/reset_room", "/controls/menu"
};

class Player : public Node, public PhysicsObject {
	Indexer *collisionOn;
	Indexer *collisionOff;
	DirectionHandler moveInput;
	InputHandler miscInput;

	Indexer *frictionMap;
	float frictionValue = 1;
	PersonalPhysicsStats *physics = new PersonalPhysicsStats();
	GlobalPhysicsStats *globalPhysics = new GlobalPhysicsStats();
	std::vector<PersonalPhysicsStats *> colliding;

	const float scaleFactor = 3;
	sf::Vector2f enterPoint = sf::Vector2f(0,0);
	sf::Vector2f startPoint = sf::Vector2f(0,0);

	sf::RectangleShape textShape;
	sf::Text text;
	DrawNode *textBackground;
	DrawNode *textNode;
	bool textVisible = false;

	GridSection *section = NULL;
	bool lastTrigger = false;
	bool triggerOverride = false;

	Node *camera = NULL;
	float zoomLevel = 3.0;
	float zoomTarget = 3.0;

	sf::Vector2f lowCorner = sf::Vector2f(0,0);
	sf::Vector2f highCorner = sf::Vector2f(0,0);
	sf::Vector2f center = sf::Vector2f(0,0);

	int frameWidth = 0;
	int frameHeight = 0;
	int animationSet = 0;
    int maxFrames = 0;
    int frame = 0;
    double nextTime = 0;
    double delay = -1;

public:
	Node *background1 = NULL;
	Node *background2 = NULL;
	Node *background3 = NULL;
	Node *background4 = NULL;

	Player(Indexer *_collisionOn, Indexer *_collisionOff, Indexer *_friction, sf::Font *font) :
	Node(PLAYER, sf::Vector2i(22, 29)), collisionOn(_collisionOn), collisionOff(_collisionOff),
		moveInput("/movement", INPUT, this), miscInput(miscLayout, INPUT, this), frictionMap(_friction) {

		textShape.setFillColor(sf::Color(0,0,0,200));
		text.setFont(*font);
		text.setCharacterSize(25);
		text.setFillColor(sf::Color::White);
		textBackground = new DrawNode(textShape, TEXT, sf::Vector2i(16, 16), this);
		textBackground->setHidden();
		textNode = new DrawNode(text, TEXT, sf::Vector2i(16, 16), textBackground);
		textNode->setPosition(sf::Vector2f(14, 6));
		UpdateList::addNode(textBackground);
		UpdateList::addNode(textNode);

		frameWidth = 22;
		frameHeight = 29;
		maxFrames = 8;
		delay = 0.1;

		lowCorner = sf::Vector2f(0, _collisionOff->getSize().y * _collisionOff->getScale().y);
		//lowCorner.y = std::max(lowCorner.y, 4096.0f);
		highCorner = sf::Vector2f(_collisionOff->getSize().x * _collisionOff->getScale().x * 1.5, 0);
		center = sf::Vector2f(highCorner.x / 2, lowCorner.y / 2);
		std::cout << _collisionOff->getSize().x * _collisionOff->getScale().x << "," << _collisionOff->getSize().y * _collisionOff->getScale().y << "\n";

		Player *_player = this;
		miscInput.pressedFunc = [_player](int i) {
			if(i < 2) {
				_player->zoomTarget = std::clamp(2.0, _player->zoomTarget + 0.2 * (i ? 1 : -1), 300.0);
			} else if(i == 2) {
				_player->setPosition(_player->enterPoint);
				UpdateList::sendSignal(BOX, RESET_SECTION, _player->section);
			} else if(i == 3) {
				UpdateList::sendSignal(MENU, TOGGLE_MENU, _player->section);
			}
		};
		camera = new Node(INPUT, sf::Vector2i(450, 250), true, this);
		UpdateList::setCamera(_player->camera, sf::Vector2f(450, 250) * zoomLevel);

		setScale(sf::Vector2f(scaleFactor, scaleFactor));
		physics->snapSpeed = 6;
		physics->weight = 0.01;

		collideWith(BOX);
		collideWith(SECTION);
		collideWith(SIGN);
	}

	void update(double time) {
		if(startPoint == sf::Vector2f(0,0)) {
			startPoint = getPosition();
			if(Settings::getBool("/save/active")) {
				sf::Vector2f target = sf::Vector2f(0,0);
				target.x = Settings::getInt("/save/x");
				target.y = Settings::getInt("/save/y");
				setPosition(target);
			}
		}

		bool tempPlatforms = section != NULL && (section->trigger != section->invertTrigger);
		Indexer *collision = tempPlatforms ? collisionOn : collisionOff;

		sf::Vector2f input = moveInput.getDirection();
		bool jumpInput = input.y < -0.5;
		sf::Vector2f velocity = sf::Vector2f(input.x * time * 320.0f, 0);
		velocity = PlatformFrictionMovement(getPosition(), velocity, getSize(), time,
			physics->previous, collision, frictionMap, frictionValue, globalPhysics);
		velocity = PlatformGravityMovement(getPosition(), velocity, getSize(), time, jumpInput,
			collision, globalPhysics, physics, colliding);
		setPosition(getPosition() + velocity);
		colliding.clear();

		UpdateList::hideLayer(TEMPMAP, section == NULL || (!triggerOverride && section->trigger == section->invertTrigger));
		if(section != NULL && section->hasButton)
			lastTrigger = section->trigger != section->invertTrigger;

		//Flip horizontally
		if(velocity.x < 0)
			setScale(sf::Vector2f(-scaleFactor, scaleFactor));
		else if(velocity.x > 0)
			setScale(sf::Vector2f(scaleFactor, scaleFactor));

		//Animation
		if(input.y < -0.5 && velocity.y < -0.2) {
			if(animationSet != 2) {
				frame = 0;
				animationSet = 2;
			}
		} else if(velocity.y > 5 || (input.y < -0.5 && animationSet == 2 && frame > 2)) {
			if(animationSet != 2) {
				frame = 3;
				animationSet = 2;
			}
		} else if(std::abs(input.x) > 0.1) {
			if(animationSet != 1) {
				frame = 0;
				animationSet = 1;
			}
		} else
			animationSet = 0;

		if((nextTime -= time) <= 0) {
            nextTime = delay;
            frame++;

            //Reset to start frame
            if(frame == maxFrames)
                frame = 0;

            setTextureRect(sf::IntRect(frameWidth * frame, animationSet * frameHeight, frameWidth, frameHeight));
        }

		//Slide camera
		if(camera->getPosition() != sf::Vector2f(0,-64 / scaleFactor)) {
			sf::Vector2f target = sf::Vector2f(0,-64 / scaleFactor) - camera->getPosition();
			sf::Vector2f target2 = vectorLength(target, 400 * time);
			if(std::abs(target.x) > std::abs(target2.x) && std::abs(target.x) > std::abs(target2.x))
				target = target2;
			camera->setPosition(camera->getPosition() + target);
		}

		//Place backgrounds
		if(background1 != NULL) {
			background1->setPosition(sf::Vector2f(lerp(center.x, camera->getGPosition().x, 0.85), lerp(center.y+600, camera->getGPosition().y+600, 0.85)));
			background2->setPosition(sf::Vector2f(lerp(center.x, camera->getGPosition().x, 0.90), lerp(center.y+1500, camera->getGPosition().y+1500, 0.90)));
			background3->setPosition(sf::Vector2f(lerp(center.x, camera->getGPosition().x, 0.95), lerp(center.y+1500, camera->getGPosition().y+1500, 0.95)));
			background4->setPosition(sf::Vector2f(lerp(center.x, camera->getGPosition().x, 1), lerp(center.y, camera->getGPosition().y, 1)));

			//background1->setScale(sf::Vector2f(1/0.7, 1/0.7));
			background2->setScale(sf::Vector2f(1.3, 1.3));
			background3->setScale(sf::Vector2f(1.3, 1.3));
			background4->setScale(sf::Vector2f(1.3, 1.3));
		}

		//Update zoom level
		if(zoomLevel != zoomTarget) {
			zoomTarget = std::clamp(-10.0f, zoomTarget, 10.0f);
			zoomLevel += std::clamp(-0.02f, zoomTarget - zoomLevel, 0.02f);
			UpdateList::setCamera(camera, sf::Vector2f(450, 250) * zoomLevel);
		}

		if(!textVisible)
			textBackground->setHidden(true);
		textVisible = false;
	}

	void collide(Node *other) {
		if(other->getLayer() == SECTION) {
			if(section != other) {
				section = (GridSection *)other;
				enterPoint = getPosition();
				//std::cout << "Entering room\n";

				//Adjust camera
				sf::Vector2f cameraPosition = camera->getGPosition();
				if(section->grabCamera)
					camera->setParent(other);
				else
					camera->setParent(this);
				camera->setGPosition(cameraPosition);
				if(section->zoomLevel != 0)
					zoomTarget = section->zoomLevel;

				if(!triggerOverride && lastTrigger && !section->hasButton)
					triggerOverride = true;
				else {
					triggerOverride = false;
					lastTrigger = section->trigger != section->invertTrigger;
				}
			}
		} else if(other->getLayer() == SIGN) {
			if(section != NULL && section->signText != "") {
				text.setString(section->signText);
				//text.setPosition(sf::Vector2f(-16 * section->signText.length(), 32));
				textShape.setSize(sf::Vector2f(10 * section->signText.length(), 38));
				textBackground->setHidden(false);
				textBackground->setScale(getInverseScale());
				textBackground->setPosition(sf::Vector2f(-4.0 * section->signText.length(), -80) / getScale());
				textVisible = true;
			}
		} else if(other->getLayer() == BOX)
			colliding.push_back(dynamic_cast<PhysicsObject*>(other)->getPhysics());
	}

	void recieveSignal(int id, Node *sender) {
		if(id == RESET_GAME)
			setPosition(startPoint);
		else if(id == SAVE_GAME) {
			if(section->id != 0) {
				Settings::setBool("/save/active", true);
				Settings::setInt("/save/section", section->id);
				Settings::setInt("/save/x", getPosition().x);
				Settings::setInt("/save/y", getPosition().y);
			} else {
				Settings::setBool("/save/active", false);
				Settings::setInt("/save/section", 0);
				Settings::setInt("/save/x", 0);
				Settings::setInt("/save/y", 0);
			}
			Settings::save("res/settings.json");
		}
	}

	PersonalPhysicsStats *getPhysics() override {
		return physics;
	}

	float lerp(float a, float b, float f) {
	    return (a * (1.0 - f)) + (b * f);
	}
};