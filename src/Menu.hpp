class Menu : public DrawNode {
private:
	sf::RectangleShape backgroundShape;

	Node *startButton;
	Node *restartButton;
	Node *saveButton;
	Node *quitButton;

public:

	Menu(sf::Texture *buttonTextures, Node *parent) : DrawNode(backgroundShape, MENU, sf::Vector2i(512, 192), parent) {
		backgroundShape.setFillColor(sf::Color(0,0,0,200));
		backgroundShape.setSize(sf::Vector2f(512, 192));
		setHidden();
		setPosition(sf::Vector2f(0,-64));
		UpdateList::addListener(this, sf::Event::MouseButtonPressed);
		UpdateList::addNode(this);

		startButton = new Node(MENUBUTTON, sf::Vector2i(128, 48), false, this);
		startButton->setPosition(sf::Vector2f(-160, -16));
		startButton->setTexture(*buttonTextures);
		startButton->setTextureRect(sf::IntRect(0, 0, 128, 48));
		UpdateList::addNode(startButton);

		restartButton = new Node(MENUBUTTON, sf::Vector2i(128, 48), false, this);
		restartButton->setPosition(sf::Vector2f(-160, 48));
		restartButton->setTexture(*buttonTextures);
		restartButton->setTextureRect(sf::IntRect(0, 48, 128, 48));
		UpdateList::addNode(restartButton);

		saveButton = new Node(MENUBUTTON, sf::Vector2i(128, 48), false, this);
		saveButton->setPosition(sf::Vector2f(160, -16));
		saveButton->setTexture(*buttonTextures);
		saveButton->setTextureRect(sf::IntRect(0, 48*2, 128, 48));
		UpdateList::addNode(saveButton);

		quitButton = new Node(MENUBUTTON, sf::Vector2i(128, 48), false, this);
		quitButton->setPosition(sf::Vector2f(160, 48));
		quitButton->setTexture(*buttonTextures);
		quitButton->setTextureRect(sf::IntRect(0, 48*3, 128, 48));
		UpdateList::addNode(quitButton);
	}

	void recieveEvent(sf::Event event, WindowSize *windowSize) {
		if(event.type == sf::Event::MouseButtonPressed) {
			sf::Vector2f pos = windowSize->worldPos(
				event.mouseButton.x, event.mouseButton.y);
			if(!isHidden() && event.mouseButton.button == sf::Mouse::Left && getRect().contains(pos)) {
				if(startButton->getRect().contains(pos))
					pauseGame(false);
				else if(restartButton->getRect().contains(pos))
					UpdateList::sendSignal(RESET_GAME, this);
				else if(saveButton->getRect().contains(pos))
					saveGame();
				else if(quitButton->getRect().contains(pos))
					UpdateList::stopEngine();
			}
		}
	}

	void recieveSignal(int id, Node *sender) {
		if(id == TOGGLE_MENU)
			pauseGame(isHidden());
	}

	void pauseGame(bool pause) {
		setHidden(!pause);
	}

	void saveGame() {
		UpdateList::sendSignal(PLAYER, SAVE_GAME, this);
		pauseGame(false);
	}
};