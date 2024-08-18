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
	sf::Texture treeTexture;
	sf::Texture playerTexture;
	sf::Texture blocksTexture;
	sf::Texture backgroundTexture;
	UpdateList::loadTexture(&worldTexture, "res/world_tiles.png");
	UpdateList::loadTexture(&treeTexture, "res/Tree_1.png");
	UpdateList::loadTexture(&playerTexture, "res/climber_character.png");
	UpdateList::loadTexture(&blocksTexture, "res/blocks.png");
	UpdateList::loadTexture(&backgroundTexture, "res/background.png");

	//Load background
	Node background(BACKGROUND, sf::Vector2i(1920, 1080));
	background.setTexture(backgroundTexture);
	UpdateList::addNode(&background);

	//Load base tile maps
	GridManager worldGrid("res/world.json", SECTION, sf::Vector2i(64, 64));
	//GridMaker grid("res/world/start_area.txt");
	Indexer display(worldGrid.grid, displayIndex, -1);
	display.addRandomizer(&randomizerIndex);
	TileMap world(&worldTexture, 32, 32, &display, MAP);
	world.setScale(sf::Vector2f(2, 2));
	UpdateList::addNode(&world);

	//Secondary maps
	Indexer collisionMap(worldGrid.grid, collisionIndex, EMPTY, 64, 64);
	Indexer frictionMap(worldGrid.grid, frictionIndex, 100, 64, 64);
	TileMap tempMap(&worldTexture, 32, 32, new Indexer(worldGrid.grid, tempDisplayIndex, -1), TEMPMAP);
	tempMap.setScale(sf::Vector2f(2, 2));
	UpdateList::addNode(&tempMap);

	//Generate tree map
	GridMaker treeGrid(worldGrid.width/2, worldGrid.height/2);
	Indexer growthMap(worldGrid.grid, treeGrowIndex, NONE);
	int x = 0;
	while(x < worldGrid.width) {
		int y = 0;
		while(y < worldGrid.height && growthMap.getTile(sf::Vector2f(x,y)) == EMPTY)
			y += 2;

		if(growthMap.getTile(sf::Vector2f(x,y)) == TREE && growthMap.getTile(sf::Vector2f(x+1,y)) == TREE &&
			growthMap.getTile(sf::Vector2f(x,y-1)) == EMPTY && growthMap.getTile(sf::Vector2f(x+1,y-1)) == EMPTY) {
			treeGrid.setTile(x/2, std::ceil(y/2.0)-1, 'T');
			treeGrid.setTile(x/2, std::ceil(y/2.0)-2, 't');
		}
		x += (1 + std::rand() / ((RAND_MAX + 1u) / 4)) * 2;
	}
	TileMap treeMap(&treeTexture, 128, 128, new Indexer(&treeGrid, treeDisplayIndex, -1), TREEMAP);
	UpdateList::addNode(&treeMap);

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