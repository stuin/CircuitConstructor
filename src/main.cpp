//SFML headers
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Music.hpp>
#include <cstdlib>

#include "GridManager.h"
#include "indexes.h"
#include "Player.hpp"
#include "Button.hpp"
#include "Menu.hpp"

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
	sf::Texture menuButtonsTexture;
	sf::Texture backgroundTexture1;
	sf::Texture backgroundTexture2;
	sf::Texture backgroundTexture3;
	sf::Texture backgroundTexture4;
	UpdateList::loadTexture(&worldTexture, "res/textures/world_tiles.png");
	UpdateList::loadTexture(&decorTexture, "res/textures/decor_tiles.png");
	UpdateList::loadTexture(&treeTexture, "res/textures/trees.png");
	UpdateList::loadTexture(&foregroundTexture, "res/textures/foreground_tiles.png");
	UpdateList::loadTexture(&playerTexture, "res/textures/character.png");
	UpdateList::loadTexture(&blocksTexture, "res/textures/blocks.png");
	UpdateList::loadTexture(&menuButtonsTexture, "res/textures/menu_buttons.png");
	UpdateList::loadTexture(&backgroundTexture1, "res/textures/background1.png");
	UpdateList::loadTexture(&backgroundTexture2, "res/textures/background2.png");
	UpdateList::loadTexture(&backgroundTexture3, "res/textures/background3.png");
	UpdateList::loadTexture(&backgroundTexture4, "res/textures/background4.png");

	sf::Font font;
	font.loadFromFile("res/goudy_mediaeval_regular.ttf");

	//Load backgrounds
	Node background1(BACKGROUND, sf::Vector2i(4096, 4096));
	Node background2(BACKGROUND, sf::Vector2i(4096, 4096));
	Node background3(BACKGROUND, sf::Vector2i(4096, 4096));
	Node background4(BACKGROUND, sf::Vector2i(4096, 4096));
	background1.setTexture(backgroundTexture1);
	background2.setTexture(backgroundTexture2);
	background3.setTexture(backgroundTexture3);
	background4.setTexture(backgroundTexture4);
	UpdateList::addNode(&background4);
	UpdateList::addNode(&background3);
	UpdateList::addNode(&background2);
	UpdateList::addNode(&background1);

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
	//GridMaker foregroundGrid(worldGrid.width/4, worldGrid.height/4);
	Indexer growthMap(worldGrid.grid, treeGrowIndex, NONE);

	int x = std::rand() / ((RAND_MAX + 1u) / 3);
	while(x < worldGrid.width) {
		int y = 0;
		bool wide = true;
		while(y < worldGrid.height && growthMap.getTile(sf::Vector2f(x,y)) == TREEEMPTY) {
			if(growthMap.getTile(sf::Vector2f(x+1,y)) != TREEEMPTY)
				wide = false;
			y += 1;
		}

		int r = std::rand() / ((RAND_MAX + 1u) / 6);
		int t = growthMap.getTile(sf::Vector2f(x,y));
		int o = (t == SNOWTREE) ? 1 : 0;
		if(t == ROCK || t == SNOWROCK)
			decorGrid.setTile(x, y-1, 'r'+(t == SNOWROCK) ? SNOW_OFFSET : 0);
		else if((t == TREE || t == SNOWTREE) && r < 3) {
			switch(r) {
			case 0:
				decorGrid.setTile(x, y-1, 'f'+o*SNOW_OFFSET);
				break;
			case 1:
				decorGrid.setTile(x, y-1, o?'p'+SNOW_OFFSET : 'f');
				break;
			case 2:
				if(wide) {
					o *= 10;
					treeGrid.setTile(x,   y-1, 30+o);
					treeGrid.setTile(x+1, y-1, 31+o);
					treeGrid.setTile(x,   y-2, 32+o);
					treeGrid.setTile(x+1, y-2, 33+o);
				} else
					decorGrid.setTile(x, y-1, 'b'+o*SNOW_OFFSET);
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
	/*x = std::min(x, worldGrid.width);
	x = (int)(x/4)*4;
	while(x > 0) {
		int y = worldGrid.height;
		while(y > 0 && growthMap.getTile(sf::Vector2f(x,y)) != NONE &&
			growthMap.getTile(sf::Vector2f(x+1,y)) != NONE &&
			growthMap.getTile(sf::Vector2f(x+2,y)) != NONE)
			y -= 1;

		int r = std::rand() / ((RAND_MAX + 1u) / 4);
		switch(r) {
		case 0:
			foregroundGrid.setTile(x/4, std::ceil(y/4)+2, 1);
			break;
		case 1:
			foregroundGrid.setTile(x/4, std::ceil(y/4)+2, 2);
			break;
		case 2:
			foregroundGrid.setTile(x/4, std::ceil(y/4)+2, 31);
			foregroundGrid.setTile(x/4+1, std::ceil(y/4)+2, 32);
			break;
		case 3:
			foregroundGrid.setTile(x/4, std::ceil(y/4)+2, 41);
			foregroundGrid.setTile(x/4, std::ceil(y/4)+1, 42);
			break;
		}

		x -= 4 + std::rand() / ((RAND_MAX + 1u) / 4) * 4;
	}*/

	//Render decor map
	Indexer decor(&decorGrid, decorDisplayIndex, -1);
	display.addRandomizer(&decorRandomIndex);
	LargeTileMap decorMap(&decorTexture, 32, 32, &decor, TREEMAP);
	decorMap.setScales(sf::Vector2f(2, 2));
	UpdateList::addNodes(decorMap.getNodes());

	//Render forground map
	//Indexer foreground(&foregroundGrid, foregroundDisplayIndex, -1);
	//display.addRandomizer(&foregroundRandomIndex);
	//LargeTileMap foregroundMap(&foregroundTexture, 256, 256, &foreground, FOREGROUND);
	//UpdateList::addNodes(foregroundMap.getNodes());

	//Player
	Player player(collisionMap, frictionMap, &font);
	player.setTexture(playerTexture);
	player.setPosition(sf::Vector2f(0,0));
	player.background1 = &background1;
	player.background2 = &background2;
	player.background3 = &background3;
	player.background4 = &background4;
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
		else if(c == ',' || s == ',')
			new Button(pos + sf::Vector2f(32, 60), true);
		else if(c == 't') {
			treeGrid.setTile(pos.x / 64,   pos.y / 64, 50);
			treeGrid.setTile(pos.x / 64+1, pos.y / 64, 51);
			treeGrid.setTile(pos.x / 64,   pos.y / 64-1, 52);
			treeGrid.setTile(pos.x / 64+1, pos.y / 64-1, 53);
		} else if(s == 't') {
			treeGrid.setTile(pos.x / 64,   pos.y / 64, 60);
			treeGrid.setTile(pos.x / 64+1, pos.y / 64, 61);
			treeGrid.setTile(pos.x / 64,   pos.y / 64-1, 62);
			treeGrid.setTile(pos.x / 64+1, pos.y / 64-1, 63);
		} else if(c == '>' || s == '>' || c == '<' || s == '<' || c == 'f' || s == 'f') {
			Node *sign = new Node(SIGN, sf::Vector2i(64, 64));
			sign->setPosition(pos + sf::Vector2f(32, 32));
			UpdateList::addNode(sign);
		}
	});

	//Render tree map
	LargeTileMap treeMap(&treeTexture, 64, 64, new Indexer(&treeGrid, treeDisplayIndex, -1), TREEMAP);
	UpdateList::addNodes(treeMap.getNodes());

	Menu menu(&menuButtonsTexture, &player);

	//Finish engine setup
	UpdateList::staticLayer(BACKGROUND);
	UpdateList::staticLayer(PLAYER);
	UpdateList::staticLayer(INPUT);
	UpdateList::staticLayer(TEXT);
	UpdateList::staticLayer(MENU);
	UpdateList::staticLayer(MENUBUTTON);

	//Start music
	sf::Music music;
	if (!music.openFromFile("res/snow_game_jam.mp3"))
	    return -1;
	music.setLoop(true);
	music.setVolume(Settings::getInt("/music_volume"));
	music.play();

	UpdateList::startEngine("Climbing Blocks");
	return 0;
}