#pragma once
#include "utils.h"
#include "math.h"
#include "image.h"
#include "input.h"

enum direction {
	UP, DOWN, LEFT, RIGHT
};

class Sprite 
{
	public:
		Vector2 position;
		int speed;
		int direction;
		int verticalSpeed;
	
	Sprite();
	Sprite(Vector2 position);
};

class Character : public Sprite
{
	public:
		bool inMovement;
		bool isTouchingGround;
		bool doubleJump;
		bool hasKey;
		bool hasCoin;


	// CONSTRUCTORS 
	Character();
	Character(Vector2 position, int speed, int direction);

	void Move(int direction, double seconds_elapsed);
	void SpriteStage(double seconds_elapsed);
	void spriteUpdateMovement(double seconds_elapsed, GameMap* map);
	
};

