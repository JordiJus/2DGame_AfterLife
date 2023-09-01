#include "image.h"
class Tileset
{
	public:
		Image* tilemap;
		int tileset_width;
		int tileset_height;
		int numtiles;

		Tileset(Image* image, int tileset_width, int tileset_height, int numtiles);
};