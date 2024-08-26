//SFML headers
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Music.hpp>
#include <cstdlib>

#include "Skyrmion/tiling/TileFilters.hpp"

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
	sf::Texture playerTexture;
	sf::Texture blocksTexture;
	sf::Texture flagTexture;
	sf::Texture yetiTexture;
	sf::Texture tentTexture;
	sf::Texture menuButtonsTexture;
	UpdateList::loadTexture(&worldTexture, "res/textures/world_tiles.png");
	UpdateList::loadTexture(&decorTexture, "res/textures/decor_tiles.png");
	UpdateList::loadTexture(&treeTexture, "res/textures/trees.png");
	UpdateList::loadTexture(&playerTexture, "res/textures/character.png");
	UpdateList::loadTexture(&blocksTexture, "res/textures/blocks.png");
	UpdateList::loadTexture(&flagTexture, "res/textures/flag.png");
	UpdateList::loadTexture(&yetiTexture, "res/textures/sleeping_yeti.png");
	UpdateList::loadTexture(&tentTexture, "res/textures/tent.png");
	UpdateList::loadTexture(&menuButtonsTexture, "res/textures/menu_buttons.png");

	sf::Texture backgroundTexture1;
	sf::Texture backgroundTexture2;
	sf::Texture backgroundTexture3;
	sf::Texture backgroundTexture4;
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
	MapIndexer display(worldGrid.grid, displayIndex, -1);
	//RandomIndexer display(new MapIndexer(worldGrid.grid, displayIndex, -1), randomizerIndex, -1);
	LargeTileMap world(&worldTexture, 32, 32, &display, MAP);
	world.setScale(sf::Vector2f(2, 2));
	UpdateList::addNodes(worldGrid.getNodes());
	UpdateList::addNodes(world.getNodes());

	//Secondary maps
	MapIndexer *collisionMapOff = new MapIndexer(worldGrid.grid, collisionIndexOff, EMPTY, 64, 64);
	MapIndexer *collisionMapOn = new MapIndexer(worldGrid.grid, collisionIndexOn, EMPTY, 64, 64);
	MapIndexer *frictionMap = new MapIndexer(worldGrid.grid, frictionIndex, 100, 64, 64);
	LargeTileMap tempMap(&worldTexture, 32, 32, new MapIndexer(worldGrid.grid, tempDisplayIndex, -1), TEMPMAP);
	tempMap.setScale(sf::Vector2f(2, 2));
	UpdateList::addNodes(tempMap.getNodes());

	//Generate tree+decor maps
	GridMaker treeGrid(worldGrid.width, worldGrid.height);
	GridMaker decorGrid(worldGrid.width, worldGrid.height);
	MapIndexer growthMap(worldGrid.grid, treeGrowIndex, NONE);

	//std::cout << EMPTY << FULL << SLOPE_UPLEFT << SLOPE_UPRIGHT << ONEWAY_UP << "\n";

	int x = std::rand() / ((RAND_MAX + 1u) / 3);
	while(x < worldGrid.width) {
		int y = 0;
		bool wide = true;
		while(y < worldGrid.height && growthMap.getTile(sf::Vector2f(x,y)) == TREEEMPTY) {
			if(growthMap.getTile(sf::Vector2f(x+1,y)) != TREEEMPTY)
				wide = false;
			y += 1;
		}

		int r = std::rand() / ((RAND_MAX + 1u) / 8);
		int t = growthMap.getTile(sf::Vector2f(x,y));
		int o = (t == SNOWTREE) ? 1 : 0;
		if(t == ROCK || t == SNOWROCK)
			decorGrid.setTile(sf::Vector2f(x, y-1), 'r'+(t == SNOWROCK) ? SNOW_OFFSET : 0);
		else if((t == TREE || t == SNOWTREE) && r < 4) {
			switch(r) {
			case 0:
				decorGrid.setTile(sf::Vector2f(x, y-1), 'f'+o*SNOW_OFFSET);
				break;
			case 1:
				decorGrid.setTile(sf::Vector2f(x, y-1), o?'p'+SNOW_OFFSET : 'f');
				break;
			case 2:
				if(wide && growthMap.getTile(sf::Vector2f(x+1,y)) == t) {
					o *= 10;
					treeGrid.setTile(sf::Vector2f(x,   y-1), 30+o);
					treeGrid.setTile(sf::Vector2f(x+1, y-1), 31+o);
					treeGrid.setTile(sf::Vector2f(x,   y-2), 32+o);
					treeGrid.setTile(sf::Vector2f(x+1, y-2), 33+o);
				} else
					decorGrid.setTile(sf::Vector2f(x, y-1), 'r'+o*SNOW_OFFSET);
				break;
			case 3:
				decorGrid.setTile(sf::Vector2f(x, y-1), 'b'+o*SNOW_OFFSET);
				break;
			}
		} else if(wide && (t == TREE || t == SNOWTREE) && growthMap.getTile(sf::Vector2f(x+1,y)) == t) {
			o = (r > 6) ? 10 : 0;
			treeGrid.setTile(sf::Vector2f(x,   y-1), 10+o);
			treeGrid.setTile(sf::Vector2f(x+1, y-1), 11+o);
			treeGrid.setTile(sf::Vector2f(x,   y-2), 12+o);
			treeGrid.setTile(sf::Vector2f(x+1, y-2), 13+o);
			treeGrid.setTile(sf::Vector2f(x,   y-3), 14+o);
			treeGrid.setTile(sf::Vector2f(x+1, y-3), 15+o);
			treeGrid.setTile(sf::Vector2f(x,   y-4), 16+o);
			treeGrid.setTile(sf::Vector2f(x+1, y-4), 17+o);
		}

		x += 2 + std::rand() / ((RAND_MAX + 1u) / 3);
	}

	//Render decor map
	RandomIndexer decor(new MapIndexer(&decorGrid, decorDisplayIndex, -1), decorRandomIndex, -1);
	LargeTileMap decorMap(&decorTexture, 32, 32, &decor, TREEMAP);
	decorMap.setScale(sf::Vector2f(2, 2));
	UpdateList::addNodes(decorMap.getNodes());

	//Render tree map
	LargeTileMap treeMap(&treeTexture, 64, 64, new MapIndexer(&treeGrid, treeDisplayIndex, -1), TREEMAP);
	UpdateList::addNodes(treeMap.getNodes());

	//Player
	Player player(collisionMapOn, collisionMapOff, frictionMap, &font);
	player.setTexture(playerTexture);
	player.setPosition(sf::Vector2f(0,0));
	player.background1 = &background1;
	player.background2 = &background2;
	player.background3 = &background3;
	player.background4 = &background4;
	UpdateList::addNode(&player);

	//Place player and boxes
	Indexer scaleMap(worldGrid.grid, ' ', sf::Vector2i(64, 64));
	scaleMap.mapGrid([&player, &blocksTexture, &flagTexture, &yetiTexture, &tentTexture, &collisionMapOn, &collisionMapOff, &frictionMap, &treeGrid](uint c, sf::Vector2f pos) {
		uint s = c - SNOW_OFFSET;
		if(c == 'P' && player.getPosition() == sf::Vector2f(0,0))
			player.setPosition(pos + sf::Vector2f(32, 16));
		else if(c == 'w' || s == 'w' || c == 'g' || s == 'g' || c == 'i' || s == 'i' || c == 'm' || s == 'm')
			new MovableBox(collisionMapOn, collisionMapOff, frictionMap, c, pos + sf::Vector2f(32, 16), &blocksTexture);
		else if(c == '_' || s == '_')
			new Button(pos + sf::Vector2f(32, 60), false);
		else if(c == ',' || s == ',')
			new Button(pos + sf::Vector2f(32, 60), true);
		else if(c == 'f' || s == 'f') {
			AnimatedNode *flag = new AnimatedNode(flagTexture, 6, 0.1, SIGN, sf::Vector2i(30, 45));
			flag->setPosition(pos + sf::Vector2f(32, 18));
			flag->setScale(sf::Vector2f(2, 2));
			UpdateList::addNode(flag);
		} else if(c == 'E' || s == 'E') {
			AnimatedNode *yeti = new AnimatedNode(yetiTexture, 14, 0.1, ANIMATED, sf::Vector2i(40, 40));
			yeti->setPosition(pos + sf::Vector2f(16, 16));
			yeti->setScale(sf::Vector2f(2, 2));
			UpdateList::addNode(yeti);
		} else if(c == 't' || s == 't') {
			Node *tent = new Node(ANIMATED, sf::Vector2i(63, 37));
			tent->setPosition(pos + sf::Vector2f(16, 7));
			tent->setScale(sf::Vector2f(3, 3));
			tent->setTexture(tentTexture);
			tent->setTextureRect(sf::IntRect(0, (s == 't') ? 37 : 0, 63, 37));
			UpdateList::addNode(tent);
		} else if(c == '>' || s == '>' || c == '<' || s == '<' || c == 'f' || s == 'f') {
			Node *sign = new Node(SIGN, sf::Vector2i(64, 64));
			sign->setPosition(pos + sf::Vector2f(32, 32));
			UpdateList::addNode(sign);
		}
	});

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