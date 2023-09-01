#pragma once
#include "json.hpp"
#include "image.h"

enum eCellType : int {
	FLOORCORNERR , FLOOR, FLOORCORNERL, CUBE, CUBES, PLATL, PLAT, PLATR, WALLR, WALL, WALLL, CORNER1, CORNER2, FIRE1, FIRE2, FIRE3, CEILCORNERR, CEIL, CEILCORNERL , CORNER3, CORNER4, 
	BRIDGE1, BRIDGE2, BRIDGE3, CHAIN, EMPTY, DOOR, OPENDOOR, KEY, BLOOD, TOPRAIL, BOOK, ENDCHAIN, CHEST, TOMB, BROKENTOMB, TOP_STAIR, STAIR, SIGN
};

enum eItemType : int {
	NOTHING, KEY_ITEM, COIN_ITEM
};

struct sCell {
	eCellType type;
	eItemType item;
};

struct Tileset {
	Image tilemap;
	int tileset_width;
	int tileset_height;
	int numtiles;
};

class GameMap {

public:

	int width;
	int height;
	sCell* data;

	GameMap() {
		width = height = 0;
		data = NULL;
	}

	GameMap(int w, int h) {
		width = w;
		height = h;
		data = new sCell[w * h];
	}

	sCell& getCell(int x, int y) {
		return data[x + y * width];
	}

	void drawMap(Image& framebuffer, Tileset tileset, int cameraposx, int cameraposy, int cs);
};
	
GameMap* loadGameMap(const char* filename);