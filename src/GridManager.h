#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "Skyrmion/GridMaker.h"
#include "Skyrmion/VertexGraph.hpp"
#include "Skyrmion/Settings.h"
#include "Skyrmion/Node.h"

enum sides {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

class GridSection : public Vertex<4>, public DrawNode {
	int id;

public:
	std::string file;
	int tileOffset = 0;
	bool trigger = false;
	bool grabCamera = false;
	float zoomLevel = 0;

	int upId = 0;
	int rightId = 0;
	int downId = 0;
	int leftId = 0;

	int width = 0;
	int height = 0;
	int x = 0;
	int y = 0;

	sf::RectangleShape rect;

	GridSection(GridSection *root, json data, Layer layer) : Vertex(root), DrawNode(rect, layer) {

		id = data.value("id", 0);
		file = data.value("file", "");
		tileOffset = data.value("tile_offset", 0);

		upId = data.value("up", 0);
		rightId = data.value("right", 0);
		downId = data.value("down", 0);
		leftId = data.value("left", 0);

		x = data.value("x_offset", 0);
		y = data.value("y_offset", 0);

		grabCamera = data.value("grab_camera", false);
		zoomLevel = data.value("zoom", 0.0f);

		std::string line;
		std::ifstream mapFile(file);

		//Get maximum file size
		while(std::getline(mapFile, line)) {
			if(line.size() > width)
				width = line.size();
			++height;
		}
		mapFile.close();

		setSize(sf::Vector2i(width, height));
		setHidden(!Settings::getBool("/debug_sections"));
	}

	void updateSize(sf::Vector2i scale) {
		setPosition((x + width/2.0) * scale.x, (y + height/2.0) * scale.y);
		setSize(sf::Vector2i(width * scale.x, height * scale.y));
		rect.setSize(sf::Vector2f(width * scale.x, height * scale.y));
		//rect.setPosition(getPosition() - sf::Vector2f(width/2, height/2));
		rect.setOutlineColor(sf::Color::Magenta);
		rect.setOutlineThickness(1);
		rect.setFillColor(sf::Color::Transparent);
	}
};

class GridManager {
	std::vector<GridSection *> sections;
	GridSection *root;
	json world;

	int x = 0;
	int y = 0;

public:
	GridMaker *grid;
	int width = 0;
	int height = 0;

	GridManager(std::string file, Layer _layer, sf::Vector2i scale) {
		std::ifstream f(file);
		world = json::parse(f);
		root = new GridSection(NULL, world["maps"][0], _layer);
		sections.push_back(root);
		width = root->width;
		height = root->height;

		GridSection *next;
		for(int i = 1; i < world["maps"].size(); i++) {
			next = new GridSection(root, world["maps"][i], _layer);
			sections.push_back(next);
		}

		readNeighbors(0, root);
		//root->printAddress();
		width -= x;
		height -= y;
		std::cout << "\n" << x << "," << y << "," << width << "," << height << "\n";

		grid = new GridMaker(width-x, height-y);
		for(int i = 0; i < sections.size(); i++) {
			next = sections[i];
			next->x -= x;
			next->y -= y;
			next->updateSize(scale);

			//std::cout << next->file << " " << next->tileOffset << "\n";
			//std::cout << next->x << "," << next->y << "," << next->width << "," << next->height << "\n";
			grid->reload(next->file, next->tileOffset, sf::Rect<uint>(next->x, next->y, next->width, next->height));
		}
		//grid->printGrid();
	}

	void readNeighbors(int i, GridSection *prev) {
		GridSection *next;
		x = std::min(x, prev->x);
		y = std::min(y, prev->y);

		if(prev->upId != 0 && !prev->hasEdge(UP)) {
			next = sections[prev->upId];
			root->addVertex(UP, next, DOWN);

			next->x += prev->x;
			next->y += prev->y - next->height;
			y = std::min(y, next->y);

			readNeighbors(prev->upId, next);
		}
		if(prev->rightId != 0 && !prev->hasEdge(RIGHT)) {
			next = sections[prev->rightId];
			root->addVertex(RIGHT, next, LEFT);

			next->x += prev->x + prev->width;
			next->y += prev->y;
			width = std::max(width, next->x + next->width);

			readNeighbors(prev->rightId, next);
		}
		if(prev->downId != 0 && !prev->hasEdge(DOWN)) {
			next = sections[prev->downId];
			root->addVertex(DOWN, next, UP);

			next->x += prev->x;
			next->y += prev->y + prev->height;
			height = std::max(height, next->y + next->height);

			readNeighbors(prev->downId, next);
		}
		if(prev->leftId != 0 && !prev->hasEdge(LEFT)) {
			next = sections[prev->leftId];
			root->addVertex(LEFT, next, RIGHT);

			next->x += prev->x - next->width;
			next->y += prev->y;
			x = std::min(x, next->x);

			readNeighbors(prev->leftId, next);
		}
	}

	std::vector<Node *> getNodes() {
        std::vector<Node *> nodes;
        for(GridSection *section : sections)
            nodes.push_back(section);
        return nodes;
    }
};