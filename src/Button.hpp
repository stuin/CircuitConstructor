class Button : public Node {
	GridSection *section = NULL;
	bool reset = true;

public:
	Button(sf::Vector2f pos, bool player) : Node(BUTTON, sf::Vector2i(12, 4), true) {
		setPosition(pos);

		collideWith(PLAYER, player);
		collideWith(BOX);
		collideWith(SECTION);

		UpdateList::addNode(this);
	}

	void update(double time) {
		if(section != NULL) {
			if(reset)
				section->trigger = false;
			else
				section->trigger = true;
			reset = true;
		}
	}

	void collide(Node *object) {
		if(object->getLayer() == SECTION) {
			section = (GridSection *) object;
			collideWith(SECTION, false);
		} else
			reset = false;
	}

	void recieveSignal(int id, Node *sender) {
		if(id == RESET_SECTION && sender == section) {
			section->trigger = false;
		}
	}
};