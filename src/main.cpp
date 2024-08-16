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
	sf::Texture machineTexture;
	sf::Texture playerTexture;
	UpdateList::loadTexture(&worldTexture, "res/world_tiles.png");
	UpdateList::loadTexture(&machineTexture, "res/machine_tiles.png");
	UpdateList::loadTexture(&playerTexture, "res/player.png");

	//Load base tile maps
	GridMaker grid("res/start_room.txt");
	TileMap world(&worldTexture, 16, 16, new Indexer(&grid, displayIndex, 0), MAP);
	Indexer collisionMap(&grid, collisionIndex, 0, 16, 16);
	UpdateList::addNode(&world);

	//Lower area player
	Player player(collisionMap);
	player.setTexture(playerTexture);
	UpdateList::addNode(&player);

	//Place player
	collisionMap.mapGrid([&player](char c, sf::Vector2f pos) {
		if(c == 'P') {
			player.setPosition(pos + sf::Vector2f(8, 8));
		}
	});

	//Finish engine setup
	UpdateList::staticLayer(MAP);
	UpdateList::staticLayer(INPUT);
	UpdateList::setCamera(&player, sf::Vector2f(450, 250));

	UpdateList::startEngine("Circuit Constructor");
	return 0;
}