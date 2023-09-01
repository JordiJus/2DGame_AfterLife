#include "tileset.h"
#include "image.h"

Tileset::Tileset(Image* image, int tileset_width, int tileset_height, int numtiles) {
	this->tilemap = image;
	this->tileset_width = tileset_width;
	this->tileset_height = tileset_height;
	this->numtiles = numtiles;
}