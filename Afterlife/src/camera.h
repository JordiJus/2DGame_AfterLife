#pragma once
#include "game.h"
#include "stage.h"

class Camera 
{
	public:
		int posx;
		int posy;
		Vector2 topLeft;
		Vector2 bottomRight;

	//CONSTRUCTORS
	Camera();
	Camera(int posx, int posy);
	void updateCamera(int framebuffer_width, int framebuffer_height, Vector2 spritePos, int map_width, int map_heigth, int cs);
};

