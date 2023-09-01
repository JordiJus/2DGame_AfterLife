#pragma once
#define MAX_ITEMS 255
#define MAX_ENEMIES 255

#include "math.h"
#include "utils.h"
#include "game.h"
#include "image.h"
#include "sprite.h"
#include "gamemap.h"
#include "camera.h"
#include "synth.h"





class Stage
{
	public:
		Image font;
		Image minifont;
		bool isFinished;
		bool isDead;
		Color bgcolor;
		Tileset tileset;
		Tileset characters;
		Character character;
		int state;
		Synth synth;
		long time_limit;
		Synth::Sample* pickupSound;

		Stage();

		virtual void render(Image& framebuffer) {};
		virtual void update(double seconds_elapsed) {};
		virtual void initStage() {};
};

class Menu : public Stage
{
	public:
		Image* title;

		Image* start;
		Image* close;

	Menu();
	virtual void render(Image& framebuffer);
	virtual void update(double seconds_elapsed);
	virtual void initStage();

};

class Cinematic : public Stage
{
	public:
		Image* title;
		Image* cont;

	Cinematic();
	virtual void render(Image& framebuffer);
	virtual void update(double seconds_elapsed);
	virtual void initStage();


};

class Intro : public Stage
{
	public:
		
		GameMap* map;
		int textStatus;
		
		

	Intro();

	Intro(Character sprite, Color bgcolor, GameMap* map, Tileset tileset, Tileset characters);

	virtual void render(Image& framebuffer);
	virtual void update(double seconds_elapsed);
	virtual void initStage();
};

class Field : public Stage
{
	public:

		GameMap* map;
		Sprite enemy;
		Sprite key;
	
	Field();

	Field(Character sprite, Color bgcolor, GameMap* map, Tileset tileset, Tileset characters);

	virtual void render(Image& framebuffer);
	virtual void update(double seconds_elapsed);
	virtual void initStage();
};

class Cave : public Stage
{
public:

	GameMap* map;
	int isCinePlaying;
	bool canGetTime;
	Sprite lostSoul;
	

	Cave();

	Cave(Character sprite, Color bgcolor, GameMap* map, Tileset tileset, Tileset characters);

	virtual void render(Image& framebuffer);
	virtual void update(double seconds_elapsed);
	virtual void initStage();
};

class Last : public Stage
{
public:

	GameMap* map;
	Sprite coin;
	Sprite enemy;

	Last();

	Last(Character sprite, Color bgcolor, GameMap* map, Tileset tileset, Tileset characters);

	virtual void render(Image& framebuffer);
	virtual void update(double seconds_elapsed); 
	virtual void initStage();
};

class Death : public Stage
{
public:
	Image* death;
	Image* cont;

	Death();


	virtual void render(Image& framebuffer);
	virtual void update(double seconds_elapsed);
	virtual void initStage();
};

class End : public Stage
{
public:


	End();


	virtual void render(Image& framebuffer);
	virtual void update(double seconds_elapsed);
	virtual void initStage();
};