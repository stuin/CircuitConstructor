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

	//Secondary maps
	Indexer collisionMap(worldGrid.grid, collisionIndex, EMPTY, 32, 32);
	Indexer frictionMap(worldGrid.grid, frictionIndex, 100, 32, 32);
	TileMap tempMap(&worldTexture, 32, 32, new Indexer(worldGrid.grid, tempDisplayIndex, -1, 32, 32), TEMPMAP);
	UpdateList::addNode(&tempMap);

	//Player
	Player player(collisionMap, frictionMap);
	player.setTexture(playerTexture);
	player.setPosition(sf::Vector2f(0,0));
	background.setParent(&player);
	UpdateList::addNode(&player);

	//Place player and boxes
	collisionMap.mapGrid([&player, &blocksTexture, &collisionMap, &frictionMap](uint c, sf::Vector2f pos) {
		uint s = c - SNOW_OFFSET;
		if(c == 'P' && player.getPosition() == sf::Vector2f(0,0))
			player.setPosition(pos + sf::Vector2f(16, 0));
		else if(c == 'w' || c == 'g' || c == 'i' || s == 'w' || s == 'g' || s == 'i')
			new MovableBox(collisionMap, frictionMap, c, pos + sf::Vector2f(8, 8), &blocksTexture);
		else if(c == '_' || s == '_')
			new Button(pos + sf::Vector2f(16, 30), false);
	});

	//Finish engine setup
	UpdateList::staticLayer(MAP);
	UpdateList::staticLayer(PLAYER);
	UpdateList::staticLayer(INPUT);

	UpdateList::startEngine("Climbing Blocks");
	return 0;
}