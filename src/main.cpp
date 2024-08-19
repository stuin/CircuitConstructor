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
	sf::Texture decorTexture;
	sf::Texture treeTexture;
	sf::Texture foregroundTexture;
	sf::Texture playerTexture;
	sf::Texture blocksTexture;
	sf::Texture backgroundTexture;
	UpdateList::loadTexture(&worldTexture, "res/world_tiles.png");
	UpdateList::loadTexture(&decorTexture, "res/decor_tiles.png");
	UpdateList::loadTexture(&treeTexture, "res/trees.png");
	UpdateList::loadTexture(&foregroundTexture, "res/foreground_tiles.png");
	UpdateList::loadTexture(&playerTexture, "res/character.png");
	UpdateList::loadTexture(&blocksTexture, "res/blocks.png");
	UpdateList::loadTexture(&backgroundTexture, "res/background.png");

	//Load background
	Node background(BACKGROUND, sf::Vector2i(1920, 1080));
	background.setTexture(backgroundTexture);
	UpdateList::addNode(&background);

	//Load base tile maps
	GridManager worldGrid("res/world.json", SECTION, sf::Vector2i(64, 64));
	Indexer display(worldGrid.grid, displayIndex, -1);
	display.addRandomizer(&randomizerIndex);
	LargeTileMap world(&worldTexture, 32, 32, &display, MAP);
	world.setScales(sf::Vector2f(2, 2));
	UpdateList::addNodes(worldGrid.getNodes());
	UpdateList::addNodes(world.getNodes());

	//Secondary maps
	Indexer collisionMap(worldGrid.grid, collisionIndex, EMPTY, 64, 64);
	Indexer frictionMap(worldGrid.grid, frictionIndex, 100, 64, 64);
	LargeTileMap tempMap(&worldTexture, 32, 32, new Indexer(worldGrid.grid, tempDisplayIndex, -1), TEMPMAP);
	tempMap.setScales(sf::Vector2f(2, 2));
	UpdateList::addNodes(tempMap.getNodes());

	//Generate tree+decor maps
	GridMaker treeGrid(worldGrid.width, worldGrid.height);
	GridMaker decorGrid(worldGrid.width, worldGrid.height);
	GridMaker foregroundGrid(worldGrid.width/4, worldGrid.height/4);
	Indexer growthMap(worldGrid.grid, treeGrowIndex, NONE);
	int x = 0;
	while(x < worldGrid.width) {
		int y = 0;
		bool wide = true;
		while(y < worldGrid.height && growthMap.getTile(sf::Vector2f(x,y)) == EMPTY) {
			if(growthMap.getTile(sf::Vector2f(x+1,y)) != EMPTY)
				wide = false;
			y += 1;
		}

		int r = std::rand() / ((RAND_MAX + 1u) / 8);
		int t = growthMap.getTile(sf::Vector2f(x,y));
		int o = (t == SNOWTREE) ? 1 : 0;
		if(t == ROCK || t == SNOWROCK)
			decorGrid.setTile(x, y-1, 'r'+(t == SNOWROCK) ? SNOW_OFFSET : 0);
		else if((t == TREE || t == SNOWTREE) && r < 4) {
			switch(r) {
			case 0:
				decorGrid.setTile(x, y-1, 'f'+o*SNOW_OFFSET);
				break;
			case 1:
				decorGrid.setTile(x, y-1, o?'p'+SNOW_OFFSET : 'f');
				break;
			case 2:
				decorGrid.setTile(x, y-1, 'b'+o*SNOW_OFFSET);
				break;
			case 3:
				treeGrid.setTile(x, y-1, 1+o);
				break;
			}
		} else if(wide && (t == TREE || t == SNOWTREE) && growthMap.getTile(sf::Vector2f(x+1,y)) == t) {
			o *= 10;
			treeGrid.setTile(x,   y-1, 10+o);
			treeGrid.setTile(x+1, y-1, 11+o);
			treeGrid.setTile(x,   y-2, 12+o);
			treeGrid.setTile(x+1, y-2, 13+o);
			treeGrid.setTile(x,   y-3, 14+o);
			treeGrid.setTile(x+1, y-3, 15+o);
			treeGrid.setTile(x,   y-4, 16+o);
			treeGrid.setTile(x+1, y-4, 17+o);
		}

		x += 2 + std::rand() / ((RAND_MAX + 1u) / 3);
	}

	//Render decor map
	Indexer decor(&decorGrid, decorDisplayIndex, -1);
	display.addRandomizer(&decorRandomIndex);
	LargeTileMap decorMap(&decorTexture, 32, 32, &decor, TREEMAP);
	decorMap.setScales(sf::Vector2f(2, 2));
	UpdateList::addNodes(decorMap.getNodes());

	//Player
	Player player(collisionMap, frictionMap);
	player.setTexture(playerTexture);
	player.setPosition(sf::Vector2f(0,0));
	background.setParent(&player);
	UpdateList::addNode(&player);

	//Place player and boxes
	collisionMap.mapGrid([&player, &blocksTexture, &collisionMap, &frictionMap, &treeGrid](uint c, sf::Vector2f pos) {
		uint s = c - SNOW_OFFSET;
		if(c == 'P' && player.getPosition() == sf::Vector2f(0,0))
			player.setPosition(pos + sf::Vector2f(32, 16));
		else if(c == 'w' || s == 'w' || c == 'g' || s == 'g' || c == 'i' || s == 'i' || c == 'm' || s == 'm')
			new MovableBox(collisionMap, frictionMap, c, pos + sf::Vector2f(32, 16), &blocksTexture);
		else if(c == '_' || s == '_')
			new Button(pos + sf::Vector2f(32, 60), false);
		else if(c == 't')
			treeGrid.setTile(pos.x / 64, pos.y / 64, 3);
		else if(s == 't')
			treeGrid.setTile(pos.x / 64, pos.y / 64, 4);
	});

	//Render tree map
	LargeTileMap treeMap(&treeTexture, 64, 64, new Indexer(&treeGrid, treeDisplayIndex, -1), TREEMAP);
	UpdateList::addNodes(treeMap.getNodes());

	//Finish engine setup
	UpdateList::staticLayer(MAP);
	UpdateList::staticLayer(PLAYER);
	UpdateList::staticLayer(INPUT);

	UpdateList::startEngine("Climbing Blocks");
	return 0;
}