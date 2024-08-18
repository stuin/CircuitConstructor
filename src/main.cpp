//SFML headers
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <cstdlib>

#include "GridManager.h"
#include "indexes.h"
#include "Player.hpp"
#include "Button.hpp"

int main() {
	std::srand(7802);

	//Load settings file
	Settings::loadSettings("res/settings.json");

	//Load tilemap textures
	sf::Texture worldTexture;
	sf::Texture playerTexture;
	sf::Texture blocksTexture;
	sf::Texture backgroundTexture;
	UpdateList::loadTexture(&worldTexture, "res/world_tiles.png");
	UpdateList::loadTexture(&playerTexture, "res/climber_character.png");
	UpdateList::loadTexture(&blocksTexture, "res/blocks.png");
	UpdateList::loadTexture(&backgroundTexture, "res/background.png");

	//Load background
	Node background(MAP, sf::Vector2i(1920, 1080));
	background.setTexture(backgroundTexture);
	UpdateList::addNode(&background);

	//Load base tile maps
	GridManager worldGrid("res/world.json", SECTION, sf::Vector2i(32, 32));
	//GridMaker grid("res/world/start_area.txt");
	Indexer display(worldGrid.grid, displayIndex, -1);
	display.addRandomizer(&randomizerIndex);
	TileMap world(&worldTexture, 32, 32, &display, MAP);
	UpdateList::addNode(&world);

	Indexer collisionMap(worldGrid.grid, collisionIndex, EMPTY, 32, 32);
	Indexer frictionMap(worldGrid.grid, frictionIndex, 100, 32, 32);

	//Player
	Player player(collisionMap, frictionMap);
	player.setTexture(playerTexture);
	player.setPosition(sf::Vector2f(0,0));
	background.setParent(&player);
	UpdateList::addNode(&player);

	//Place player and boxes
	collisionMap.mapGrid([&player, &blocksTexture, &collisionMap, &frictionMap](uint c, sf::Vector2f pos) {
		if(c == 'P' && player.getPosition() == sf::Vector2f(0,0)) {
			player.setPosition(pos + sf::Vector2f(16, 0));
		} else if(c == 'w' || c == 'g' || c == 'i' ||
			c == 'w' + SNOW_OFFSET || c == 'g' + SNOW_OFFSET || c == 'i' + SNOW_OFFSET) {
			MovableBox *b = new MovableBox(collisionMap, frictionMap, pos + sf::Vector2f(8, 8), sf::Vector2i(16, 16));
			b->setScale(sf::Vector2f(1.5, 1.5));
			b->setTexture(blocksTexture);
			if(c == 'i' || c == 'i' + SNOW_OFFSET) {
				b->setTextureRect(sf::IntRect(0, 16, 16, 16));
				b->frictionValue = 0.5;
			} else {
				b->setTextureRect(sf::IntRect(
					(c == 'w' || c == 'w' + SNOW_OFFSET) ? 48 : 32, 0, 16, 16));
			}
			UpdateList::addNode(b);
		} else if(c == '_' || c == '_' + SNOW_OFFSET)
			new Button(pos + sf::Vector2f(16, 30), false);
	});

	//Finish engine setup
	UpdateList::staticLayer(MAP);
	UpdateList::staticLayer(PLAYER);
	UpdateList::staticLayer(INPUT);

	UpdateList::startEngine("Climbing Blocks");
	return 0;
}