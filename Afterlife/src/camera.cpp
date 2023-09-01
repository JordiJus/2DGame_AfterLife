#include "camera.h"
#include "game.h"

Camera::Camera() {
	this->posx = 0;
	this->posy = 0;
}

Camera::Camera(int posx, int posy) {
	this->posx = posx;
	this->posy = posy;
}

void Camera::updateCamera(int framebuffer_width, int framebuffer_height, Vector2 spritePos, int map_width, int map_heigth, int cs) {
	int centerx = framebuffer_width / 2;
	int centery = framebuffer_height / 2;

	this->posx = spritePos.x - centerx;
	this->posy = spritePos.y - centery; 

	this->posx = max(0, this->posx);
	this->posy = max(0, this->posy);
	this->posx = min(map_width * cs - framebuffer_width, this->posx);
	this->posy = min(map_heigth * cs - framebuffer_height, this->posy);
}