//SFML headers
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Skyrmion/InputHandler.h"
#include "indexes.h"
#include "Player.hpp"

int main() {
	//Load settings file
	Settings::loadSettings("res/settings.json");

	//Load tilemap textures
	sf::Texture worldTexture;
	sf::Texture playerTexture;
	sf::Texture crateTexture;
	sf::Texture backgroundTexture;
	UpdateList::loadTexture(&worldTexture, "res/world_tiles.png");
	UpdateList::loadTexture(&playerTexture, "res/dev_player.png");
	UpdateList::loadTexture(&crateTexture, "res/crate.png");
	UpdateList::loadTexture(&backgroundTexture, "res/background.png");

	//Load background
	Node background(MAP, sf::Vector2i(1920, 1080));
	background.setTexture(backgroundTexture);
	UpdateList::addNode(&background);

	//Load base tile maps
	GridMaker grid("res/world/start_area.txt");
	TileMap world(&worldTexture, 32, 32, new Indexer(&grid, displayIndex, 0), MAP);
	Indexer collisionMap(&grid, collisionIndex, 0, 32, 32);
	UpdateList::addNode(&world);

	//Player
	Player player(collisionMap);
	player.setTexture(playerTexture);
	background.setParent(&player);
	UpdateList::addNode(&player);

	//Place player and boxes
	collisionMap.mapGrid([&player, &crateTexture, &collisionMap](char c, sf::Vector2f pos) {
		if(c == 'P') {
			player.setPosition(pos + sf::Vector2f(16, 0));
		} if(c == 'w') {
			MovableBox *b = new MovableBox(collisionMap, pos + sf::Vector2f(16, 16), sf::Vector2i(32, 32));
			b->setTexture(crateTexture);
			UpdateList::addNode(b);
		}
	});

	//Finish engine setup
	UpdateList::staticLayer(MAP);
	UpdateList::staticLayer(INPUT);

	UpdateList::startEngine("Climbing Blocks");
	return 0;
}