#include "sprite.h"
#include "game.h"

Sprite::Sprite() {

}


Sprite::Sprite(Vector2 position) {
	this->position = position;
}

Character::Character() {
	this->position = Vector2(0, 0);
	this->speed = 0;
	this->direction = 0;
	this->inMovement = false;
	this->verticalSpeed = 80;
	this->isTouchingGround = true;
	this->doubleJump = false;

}

Character::Character(Vector2 position, int speed, int direction) {
	this->position = position;
	this->speed = speed;
	this->direction = direction;
	this->inMovement = false;
	this->verticalSpeed = 80;
	this->isTouchingGround = true;
	this->doubleJump = true;
	this->hasCoin = false;
	this->hasKey = false;
}

void Character::Move(int direction, double seconds_elapsed) {
	if (direction == UP) {
		this->position.y -= this->speed * seconds_elapsed;
		this->inMovement = true;
	}
	if (direction == DOWN) {
		this->position.y += this->speed * seconds_elapsed;
		this->inMovement = true;
	}
	if (direction == LEFT) {
		this->position.x -= this->speed * seconds_elapsed;
		this->direction = direction;
		this->inMovement = true;
	}
	if (direction == RIGHT) {
		this->position.x += this->speed * seconds_elapsed;
		this->direction = direction;
		this->inMovement = true;
	}

}

void Character::spriteUpdateMovement(double seconds_elapsed, GameMap* map) {
	int cellX = (this->position.x + 8) / 16;
	int cellY = (this->position.y + 8) / 16;

	int cellYGround = (this->position.y + 13) / 16;
	int cellYTop = (this->position.y) / 16;

	int cellXR = (this->position.x + 12) / 16;
	int cellXL = (this->position.x + 5) / 16;

	eCellType& typeR = map->getCell(cellXR, cellY).type;
	eCellType& typeL = map->getCell(cellXL, cellY).type;

	eCellType& typeTop = map->getCell(cellX, cellYTop).type;
	eCellType& typeGround = map->getCell(cellX, cellYGround).type;
	eCellType& type = map->getCell(cellX, cellY).type;
	if (typeGround != FLOOR && typeGround != STAIR && typeGround != TOP_STAIR && typeGround != FLOORCORNERL && typeGround != FLOORCORNERR && typeGround != PLATR && typeGround != PLATL && typeGround != PLAT || (type == PLATR || type == PLATL || type == PLAT)) {
		this->position.y += 30 * seconds_elapsed;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_UP) && (type == STAIR || type == TOP_STAIR || typeGround == TOP_STAIR)) //if key up
	{
		this->Move(UP, seconds_elapsed);
		this->inMovement = true;
		//sprite.isTouchingGround = false;
		//ground = sprite.position.y;
	}
	else if (Input::isKeyPressed(SDL_SCANCODE_UP) && this->isTouchingGround == true && type != PLATR) //if key up
	{
		//sprite.Move(UP, seconds_elapsed);
		this->inMovement = true;
		this->isTouchingGround = false;
		this->doubleJump = false;
		//ground = sprite.position.y+16;
		Game::instance->synth.playSample("data/Jump.wav", 1, false);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN) && (type == STAIR || type == TOP_STAIR || typeGround == TOP_STAIR || type == PLATR || typeGround == PLATR || type == PLAT || typeGround == PLAT || type == PLATL || typeGround == PLATL)) //if key down
	{
		this->Move(DOWN, seconds_elapsed);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT) && typeR != WALLR && typeR != FLOORCORNERR && typeR != CEILCORNERR) //if key up
	{
		if (this->position.x < ((map->width * 16) - 12)) {
			this->Move(RIGHT, seconds_elapsed);
		}
	}
	if (Input::isKeyPressed(SDL_SCANCODE_LEFT) && typeL != WALLL && typeL != FLOORCORNERL && typeL != CEILCORNERL) //if key down
	{
		if (this->position.x > -6) {
			this->Move(LEFT, seconds_elapsed);
		}

	}

	if (this->isTouchingGround == false) {
		this->inMovement = true;
		this->position.y -= this->verticalSpeed * seconds_elapsed;
		if (typeTop == CEIL || typeTop == CEILCORNERR || typeTop == CEILCORNERL || type == PLATR || type == PLAT || type == PLATL) {
			this->verticalSpeed = 0;
		}
		this->verticalSpeed -= 100 * seconds_elapsed;
		if (this->verticalSpeed < 0 && (typeGround == FLOOR || typeGround == STAIR || typeGround == TOP_STAIR || typeGround == FLOORCORNERL || typeGround == FLOORCORNERR || typeGround == PLATR || typeGround == PLAT || typeGround == PLATL)) {
			this->isTouchingGround = true;
			this->doubleJump = true;
			this->verticalSpeed = 90;
		}



	}

	//example of 'was pressed'
	if (Input::wasKeyPressed(SDL_SCANCODE_X) && this->doubleJump == false) //if key A was pressed
	{
		this->verticalSpeed = 100;
		this->doubleJump = true;
		Game::instance->synth.playSample("data/DoubleJump.wav", 0.5, false);
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) //if key Z was pressed
	{
		
	}

}