#include "gamemap.h"
#include "json.hpp"
#include "math.h"
#include <fstream>

void GameMap::drawMap(Image& framebuffer, Tileset tileset, int cameraposx, int cameraposy, int cs) {

	

	// For every cell
	for (int x = 0; x < this->width; ++x)
		for (int y = 0; y < this->height; ++y)
		{
			// Get cell info
			sCell& cell = this->getCell(x, y);
			if (cell.type == -1)
				continue;
			int type = (int)cell.type;

			// Compute tile pos in tileset image
			int tilex = (type % tileset.numtiles) * cs;
			int tiley = floor(type / tileset.numtiles) * cs;

			// Create tile area
			Area area(tilex, tiley, cs, cs);

			// Place offset here if you want
			int screenx = x * cs - cameraposx;
			int screeny = y * cs - cameraposy;



			// Avoid rendering out of screen stuff
			if (screenx < -cs || screenx >(int)framebuffer.width ||
				screeny < -cs || screeny >(int)framebuffer.height)
				continue;

			// Draw region of tileset inside framebuffer
			framebuffer.drawImage(tileset.tilemap, screenx, screeny, area);
		}
}

GameMap* loadGameMap(const char* filename) {
	using json = nlohmann::json;
	std::ifstream f(filename);
	if (!f.good())
		return nullptr;
	json jData = json::parse(f);

	int w = jData["width"];
	int h = jData["height"];

	GameMap* map = new GameMap(w, h);

	for (int x = 0; x < map->width; x++) {
		for (int y = 0; y < map->height; y++) {
			int index = x + y * map->width;
			json layer = jData["layers"][0];
			int type = layer["data"][index].get<int>();
			map->getCell(x, y).type = eCellType(type - 1);
		}
	}

	return map;
}