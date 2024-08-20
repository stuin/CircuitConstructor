#include "Skyrmion/InputHandler.h"

#include "MovableBox.hpp"

std::vector<std::string> miscLayout = {
	"/controls/zoom_in", "/controls/zoom_out", "/controls/reset_room", "/controls/menu"
};

class Player : public GravityNode {
	DirectionHandler moveInput;
	InputHandler miscInput;

	const int scaleFactor = 3;
	sf::Vector2f enterPoint = sf::Vector2f(0,0);
	sf::Vector2f startPoint = sf::Vector2f(0,0);

	sf::RectangleShape textShape;
	sf::Text text;
	DrawNode *textBackground;
	DrawNode *textNode;
	bool textVisible = false;

	Node *camera = NULL;
	float zoomLevel = 3.0;
	float zoomTarget = 3.0;

	bool lastTrigger = false;
	bool triggerOverride = false;

public:

	Player(Indexer _collision, Indexer _friction, sf::Font *font) : GravityNode(_collision, _friction, PLAYER, sf::Vector2i(17, 26)),
		moveInput("/movement", INPUT, this), miscInput(miscLayout, INPUT, this) {

		textShape.setFillColor(sf::Color(0,0,0,200));
		text.setFont(*font);
		text.setCharacterSize(25);
		text.setFillColor(sf::Color::White);
		textBackground = new DrawNode(textShape, TEXT, sf::Vector2i(16, 16), this);
		textBackground->setHidden();
		textNode = new DrawNode(text, TEXT, sf::Vector2i(16, 16), textBackground);
		textNode->setPosition(sf::Vector2f(8, 0));
		UpdateList::addNode(textBackground);
		UpdateList::addNode(textNode);

		Player *_player = this;
		miscInput.pressedFunc = [_player](int i) {
			if(i < 2)
				_player->zoomTarget = std::clamp(2.0, _player->zoomTarget + 0.2 * (i ? 1 : -1), 300.0);
			else if(i == 2) {
				_player->setPosition(_player->enterPoint);
				UpdateList::sendSignal(BOX, RESET_SECTION, _player->section);
			} else if(i == 3) {
				UpdateList::sendSignal(MENU, TOGGLE_MENU, _player->section);
			}
		};
		camera = new Node(INPUT, sf::Vector2i(450, 250), true, this);
		UpdateList::setCamera(_player->camera, sf::Vector2f(450, 250) * zoomLevel);

		setScale(sf::Vector2f(scaleFactor, scaleFactor));
		isPlayer = true;
		snapSpeed = 6;
		weight = 0.01;

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

		sf::Vector2f velocity = gravityVelocity(moveInput.getDirection() * 320.0f, time);
		setPosition(getPosition() + velocity);

		UpdateList::hideLayer(TEMPMAP, section == NULL || (!triggerOverride && section->trigger == section->invertTrigger));
		if(section != NULL && section->hasButton)
			lastTrigger = section->trigger != section->invertTrigger;

		if(velocity.x < 0)
			setScale(sf::Vector2f(-scaleFactor, scaleFactor));
		else if(velocity.x > 0)
			setScale(sf::Vector2f(scaleFactor, scaleFactor));

		//Slide camera
		if(camera->getPosition() != sf::Vector2f(0,-64)) {
			sf::Vector2f target = sf::Vector2f(0,-64) - camera->getPosition();
			sf::Vector2f target2 = vectorLength(target, 400 * time);
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
			if(section != NULL) {
				text.setString(section->signText);
				//text.setPosition(sf::Vector2f(-16 * section->signText.length(), 32));
				textShape.setSize(sf::Vector2f(10 * section->signText.length(), 38));
				textBackground->setHidden(false);
				textBackground->setPosition(sf::Vector2f(-4.0 * section->signText.length(), -80));
				textVisible = true;
			}
		} else
			addCollision((GravityNode*)other);
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
};