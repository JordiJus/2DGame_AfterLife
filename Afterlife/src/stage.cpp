#include "stage.h"
#include "game.h"
#include "image.h"
#include "utils.h"
#include "gamemap.h"
#include "input.h"
#include "synth.h"
#include "camera.h"


Camera camera;


Stage::Stage()
{
	this->isFinished = false;
	this->isDead = false;
	font.loadTGA("data/bitmap-font-white.tga"); //load bitmap-font image
	minifont.loadTGA("data/mini-font-white-4x6.tga"); //load bitmap-font image
	this->pickupSound = synth.loadSample("data/Pickup.wav");

}

//MENU STAGE
Menu::Menu() {
	this->isFinished = false;
	this->state = 0;

	start = new Image((5 * 5 + 2 * 5), 9);
	start->drawText("Start", 0, 0, font);


	close = new Image((5 * 4 + 2 * 4), 9);
	close->drawText("Exit", 0, 0, font);;

	title = new Image((5 * 9 + 2 * 8), 9);
	title->drawText("AFTERLIFE", 0, 0, font);

}

void Menu::initStage() {
	this->isDead = false;
	this->isFinished = false;
	this->state = 0;
}

void Menu::render(Image& framebuffer)
{

	title->changeColor(Color(255, 50, 50));
	start->changeColor(Color(50, 50, 50));
	close->changeColor(Color(50, 50, 50));

	
	int shake = 1;
	
	if (state == 0) {

		start->changeColor(Color(150, 70, 70));

	}
	else if (state == 1) {
		close->changeColor(Color(150, 70, 70));

	}

	framebuffer.drawImage(*title, (framebuffer.width / 2) - (5 * 9 + 2 * 9) / 2, 20);
	framebuffer.drawImage(*start, (framebuffer.width / 2) - (5 * 5 + 2 * 5) / 2, (((int)(Game::instance->time*1.5) % 2) == 1 && state == 0) ? 70 + shake : 70);
	framebuffer.drawImage(*close, (framebuffer.width / 2) - (5 * 4 + 2 * 4) / 2, (((int)(Game::instance->time*1.5) % 2) == 1 && state == 1) ? 100 + shake : 100);


}

void Menu::update(double seconds_elapsed) {
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) //if key Z was pressed
	{
		if (state == 0){
			this->isFinished = true;
		}
		else if (state == 1) {
			Game::instance->must_exit = true;
		}
		
	}
	if (Input::isKeyPressed(SDL_SCANCODE_UP)) //if key up
	{
		if (state == 1){
			state = 0;
		}
	}
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key up
	{
		if (state == 0){
			state = 1;
		}
	}
}

//CINEMATIC STAGE

Cinematic::Cinematic() {
	//this->bgcolor = Color(0, 0, 0);
	cont = new Image((5 * 19 + 2 * 19), 9);
	cont->drawText("PRESS Z TO CONTINUE", 0, 0, font);
	cont->changeColor(Color(150, 150, 150));
	state = 0;
}

void Cinematic::initStage() {
	this->isDead = false;
	this->isFinished = false;
	this->state = 0;
	title = new Image((5 * 9 + 2 * 8), 9);
	title->drawText("AFTERLIFE", 0, 0, font);
}

void Cinematic::render(Image& framebuffer) {
	int shake = 1;
	framebuffer.drawImage(*cont, (framebuffer.width / 2) - (5 * 19 + 2 * 19) / 2, (((int)(Game::instance->time * 1.5) % 2) == 1) ? 110 + shake : 110);
	switch (state) {
	case 2:
		framebuffer.drawText("And if they die there", (framebuffer.width / 2) - (5 * 21 + 2 * 21) / 2, 70, font);
		framebuffer.drawText("they die in real life", (framebuffer.width / 2) - (5 * 21 + 2 * 21) / 2, 80, font);
	case 1:
		framebuffer.drawText("Some dream about an", (framebuffer.width / 2) - (5 * 19 + 2 * 19) / 2, 40, font);
		framebuffer.drawImage(*title, (framebuffer.width / 2) - (5 * 9 + 2 * 9) / 2, 50);
	case 0:
		framebuffer.drawText("Some people have", (framebuffer.width / 2) - (5 * 16 + 2 * 16) / 2, 0, font);
		framebuffer.drawText("nightmares when they", (framebuffer.width / 2) - (5 * 20 + 2 * 20) / 2, 10, font);
		framebuffer.drawText("go to sleep", (framebuffer.width / 2) - (5 * 11 + 2 * 11) / 2, 20, font);
	}
}

void Cinematic::update(double seconds_elapsed) {
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) //if key Z was pressed
	{
		state += 1;
		if (state == 3) {
			this->isFinished = true;
		}
		
	}
}




//INTRO STAGE

Intro::Intro() {
	this->character = Character();
	this->bgcolor = Color(0, 0, 0);
	

}

void Intro::initStage() {
	this->isDead = false;
	this->isFinished = false;
	this->character.position = Vector2(40, 211);
	this->textStatus = 0;
	state = 0;
}

Intro::Intro(Character sprite, Color bgcolor, GameMap* map, Tileset tileset, Tileset characters) {
	this->character = sprite;
	this->bgcolor = bgcolor;
	this->map = map;
	this->tileset = tileset;
	this->characters = characters;
	this->isFinished = false;

};

void Intro::render(Image& framebuffer)
{
	// Size in pixels of a cell
	int cs = tileset.tileset_width / tileset.numtiles;

	
	camera.updateCamera(framebuffer.width, framebuffer.height, character.position, map->width, map->height, cs);

	framebuffer.fill(bgcolor);
	map->drawMap(framebuffer, tileset, camera.posx, camera.posy, cs);


	framebuffer.drawText("Use arrows to move and jump", 20 - camera.posx, 240 - camera.posy, minifont, 4, 6, 32);
	if (textStatus == 0) {
		framebuffer.drawText("Use Z to interact", 0 - camera.posx, 110 - camera.posy, minifont, 4, 6, 32);
		
	}
	else if (textStatus == 1) {
		if (time_limit < getTime()) {
			time_limit = getTime() + 2 * 1000;
			state += 1;
		}
		else if (state == 1) {
			framebuffer.drawImage(characters.tilemap, 32 - camera.posx, 131 - camera.posy, Area(80, 64, 16, 16));
		}
		else if (state == 2) {
			framebuffer.drawText("Hi!", 36 - camera.posx, 110 - camera.posy, minifont, 4, 6, 32);
		}
		else if (state == 3) {
			framebuffer.drawText("Welcome to the afterlife", 30 - camera.posx, 110 - camera.posy, minifont, 4, 6, 32);
		}
		else if (state == 4) {
			framebuffer.drawText("Never give up", 30 - camera.posx, 110 - camera.posy, minifont, 4, 6, 32);
			framebuffer.drawText("because when you do", 30 - camera.posx, 118 - camera.posy, minifont, 4, 6, 32);
		}
		else if (state == 5) {
			framebuffer.drawText("You turn into a slime", 30 - camera.posx, 110 - camera.posy, minifont, 4, 6, 32);
		}
		else if (state == 6) {
			framebuffer.drawText("Try to use X to double jump", 30 - camera.posx, 110 - camera.posy, minifont, 4, 6, 32);
		}
		else if (state == 7) {
			framebuffer.drawText("And reach the end to", 30 - camera.posx, 110 - camera.posy, minifont, 4, 6, 32);
			framebuffer.drawText("wake up no matter what", 30 - camera.posx, 118 - camera.posy, minifont, 4, 6, 32);
		}	
	}
	if (state > 1) {
		framebuffer.drawImage(characters.tilemap, 32 - camera.posx, 131 - camera.posy, Area(((int)(Game::instance->time * 4.f) % 4) * 16.f, 64, 16, 16));
	}

	int spritex = character.position.x - camera.posx;
	int spritey = character.position.y - camera.posy;
	spritex = clamp(spritex, -6, 148);
	spritey = clamp(spritey, -3, 107);
	int flip = 0;
	character.direction == RIGHT ? flip = 1 : flip = 0;
	framebuffer.drawImage(characters.tilemap, spritex, spritey, Area(character.inMovement ? ((int)(Game::instance->time * 4.f) % 4) * 16.f : 0, 16 * flip, 16, 16));	//draws only a part of an image	

	
	/*
	int spritex = sprite.position.x - camera.posx;
	int spritey = sprite.position.y - camera.posy;
	spritex = clamp(spritex, 0, framebuffer.width);
	spritey = clamp(spritey, 0, framebuffer.height);
	framebuffer.drawImage(characters.tilemap, spritex, spritey, Area(0, 16, 16, 16);
	*/
}


void Intro::update(double seconds_elapsed) {
	character.inMovement = false;
	character.spriteUpdateMovement(seconds_elapsed, map);

	int cellX = (character.position.x + 8) / 16;
	int cellY = (character.position.y + 8) / 16;

	eCellType& type = map->getCell(cellX, cellY).type;

	if (character.position.x >= map->width * 16 - 13) {
		this->isFinished = true;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) //if key Z was pressed
	{
		if (type == TOMB) {
			textStatus = 1;
		}
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_A)) //if key Z was pressed
	{
		this->isFinished = true;

	}
}


// FIELD STAGE

Field::Field() {
	this->character = Character();
	this->bgcolor = Color(0, 0, 0);
}

Field::Field(Character sprite, Color bgcolor, GameMap* map, Tileset tileset, Tileset characters) {
	this->character = sprite;
	this->bgcolor = bgcolor;
	this->map = map;
	this->tileset = tileset;
	this->characters = characters;
	this->isFinished = false;

}

void Field::initStage() {
	this->enemy = Sprite(Vector2(100, 176));
	this->enemy.speed = 15;
	this->enemy.direction = RIGHT;
	this->enemy.verticalSpeed = 20;
	this->key = Sprite(Vector2(0,16));
	this->character.position = Vector2(0, 147);
	this->isDead = false;
	this->isFinished = false;
	this->character.hasKey = false;

}

void Field::render(Image& framebuffer) {
	// Size in pixels of a cell
	int cs = tileset.tileset_width / tileset.numtiles;

	int cellX = (character.position.x + 8) / 16;
	int cellY = (character.position.y + 8) / 16;

	eCellType& type = map->getCell(cellX, cellY).type;


	camera.updateCamera(framebuffer.width, framebuffer.height, character.position, map->width, map->height, cs);

	framebuffer.fill(bgcolor);
	map->drawMap(framebuffer, tileset, camera.posx, camera.posy, cs);

	if (character.hasKey == false) {
		this->key.position.x = 0 + -camera.posx;
		this->key.position.y = ((((int)(Game::instance->time * 1.5) % 2) == 1) ? 15 : 16) - camera.posy;
		framebuffer.drawImage(tileset.tilemap, this->key.position.x, this->key.position.y, Area(64, 48, 16, 16));
	}


	int enemyx = enemy.position.x - camera.posx;
	int enemy_flip = 0;
	int enemyy = enemy.position.y - camera.posy;
	
	enemy.direction == RIGHT ? enemy_flip = 1 : enemy_flip = 0;
	framebuffer.drawImage(characters.tilemap, enemyx, enemyy, Area(((int)(Game::instance->time * 2.f) % 2) * 16.f, 32 + 16 * enemy_flip, 16, 16));


	if (type == DOOR && character.hasKey == false) {
		framebuffer.drawText("You need the key", 180 - camera.posx, 74 - camera.posy, minifont, 4, 6, 32);
	}

	int spritex = character.position.x - camera.posx;
	int spritey = character.position.y - camera.posy;
	spritex = clamp(spritex, -6, 148);
	spritey = clamp(spritey, -3, 107);
	int flip = 0;
	character.direction == RIGHT ? flip = 1 : flip = 0;
	framebuffer.drawImage(characters.tilemap, spritex, spritey, Area(character.inMovement ? ((int)(Game::instance->time * 4.f) % 4) * 16.f : 0, 16 * flip, 16, 16));	//draws only a part of an image	

	
}

void Field::update(double seconds_elapsed) {
	character.inMovement = false;

	character.spriteUpdateMovement(seconds_elapsed, map);

	int cellX = (character.position.x + 8) / 16;
	int cellY = (character.position.y + 8) / 16;

	int cellYGround = (character.position.y + 13) / 16;
	int cellYTop = (character.position.y) / 16;

	int cellXR = (character.position.x + 12) / 16;
	int cellXL = (character.position.x + 5) / 16;

	eCellType& type = map->getCell(cellX, cellY).type;

	if (character.position.x + 8 > enemy.position.x && character.position.x + 8 < enemy.position.x + 16 && character.position.y + 8 > enemy.position.y+6 && character.position.y + 8 < enemy.position.y +20) {
		this->isDead = true;
	}
	if (character.position.x + 8 > key.position.x && character.position.x + 8 < key.position.x + 16 && character.position.y + 8 > key.position.y && character.position.y + 8 < key.position.y + 16) {
		if (character.hasKey == false) {
			Game::instance->synth.playSample(pickupSound, 1, false);
		}
		character.hasKey = true;
	}

	if (enemy.position.x > 132) {
		enemy.speed = -15;
		enemy.direction = LEFT;
		enemy.verticalSpeed = 20;
		enemy.position.y = 176;
	}
	else if (enemy.position.x < 100) {
		enemy.speed = 15;
		enemy.direction = RIGHT;
		enemy.verticalSpeed = 20;
		enemy.position.y = 176;
	}
	if (enemy.position.x < 116 && enemy.direction == LEFT) {
		enemy.verticalSpeed = -20;
	}
	else if (enemy.position.x > 116 && enemy.direction == RIGHT) {
		enemy.verticalSpeed = -20;
	}

	enemy.position.y -= enemy.verticalSpeed * seconds_elapsed;
	enemy.position.y = clamp(enemy.position.y, 100, 176);
	
	



	enemy.position.x += enemy.speed * seconds_elapsed;
	
	
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) //if key Z was pressed
	{
		if (type == DOOR && character.hasKey == true) {
			character.hasKey = false;
			this->isFinished = true;
		}
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_A)) //if key Z was pressed
	{
		this->isFinished = true;

	}
}


// CAVE STAGE

Cave::Cave() {
	this->character = Character();
	this->bgcolor = Color(0, 0, 0);
}

Cave::Cave(Character sprite, Color bgcolor, GameMap* map, Tileset tileset, Tileset characters) {
	this->character = sprite;
	this->character.position = Vector2(47, 131);
	this->bgcolor = bgcolor;
	this->map = map;
	this->tileset = tileset;
	this->characters = characters;
	this->isFinished = false;
	this->lostSoul = Sprite(Vector2(142, 132));
}

void Cave::initStage() {
	this->isDead = false;
	this->isFinished = false;
	this->character.position = Vector2(47, 131);
	this->lostSoul.position = Vector2(142, 132);
	this->isCinePlaying = 0;
	this->canGetTime = false;

	time_limit = getTime();
	state = 0;
}

void Cave::render(Image& framebuffer) {
	// Size in pixels of a cell
	int cs = tileset.tileset_width / tileset.numtiles;


	camera.updateCamera(framebuffer.width, framebuffer.height, character.position, map->width, map->height, cs);

	framebuffer.fill(bgcolor);
	map->drawMap(framebuffer, tileset, camera.posx, camera.posy, cs);


	
	if (this->isCinePlaying == 1) {
		if (time_limit < getTime()) {
			time_limit = getTime() + 1.3 * 1000.f;
			state += 1;
		}
		else if (state == 1) {
			framebuffer.drawImage(characters.tilemap, lostSoul.position.x - camera.posx, lostSoul.position.y - camera.posy, Area(((int)(Game::instance->time * 4.f) % 4) * 16.f, 64, 16, 16));
			framebuffer.drawText("This is where you", 120 - camera.posx, 114 - camera.posy, minifont, 4, 6, 32);
			framebuffer.drawText("can give up", 120 - camera.posx, 122 - camera.posy, minifont, 4, 6, 32);
			
		}
		else if (state == 2) {
			framebuffer.drawImage(characters.tilemap, lostSoul.position.x - camera.posx, lostSoul.position.y - camera.posy, Area(((int)(Game::instance->time * 4.f) % 4) * 16.f, 64, 16, 16));
			framebuffer.drawText("But that's a one", 120 - camera.posx, 114 - camera.posy, minifont, 4, 6, 32);
			framebuffer.drawText("way road", 120 - camera.posx, 122 - camera.posy, minifont, 4, 6, 32);
		}
		else if (state == 3) {
			framebuffer.drawImage(characters.tilemap, lostSoul.position.x - camera.posx, lostSoul.position.y - camera.posy, Area(((int)(Game::instance->time * 4.f) % 4) * 16.f, 64, 16, 16));
			framebuffer.drawText("To escape this", 120 - camera.posx, 114 - camera.posy, minifont, 4, 6, 32);
			framebuffer.drawText("land of death", 120 - camera.posx, 122 - camera.posy, minifont, 4, 6, 32);
		}
		else if (state == 4) {
			framebuffer.drawImage(characters.tilemap, lostSoul.position.x - camera.posx, lostSoul.position.y - camera.posy, Area(((int)(Game::instance->time * 4.f) % 4) * 16.f, 64, 16, 16));
			framebuffer.drawText("Take the coin and", 120 - camera.posx, 114 - camera.posy, minifont, 4, 6, 32);
			framebuffer.drawText("pay the toll", 120 - camera.posx, 122 - camera.posy, minifont, 4, 6, 32);
		}
		else if (state == 5) {
			framebuffer.drawImage(characters.tilemap, lostSoul.position.x - camera.posx, lostSoul.position.y - camera.posy, Area(((int)(Game::instance->time * 4.f) % 4) * 16.f, 64, 16, 16));
			framebuffer.drawText("With the coin", 120 - camera.posx, 114 - camera.posy, minifont, 4, 6, 32);
			framebuffer.drawText("press Z on the sun", 120 - camera.posx, 122 - camera.posy, minifont, 4, 6, 32);
		}
		else if (state == 6) {
			framebuffer.drawImage(characters.tilemap, lostSoul.position.x - camera.posx, lostSoul.position.y - camera.posy, Area(((int)(Game::instance->time * 4.f) % 4) * 16.f, 64, 16, 16));
			framebuffer.drawText("And wake up from", 120 - camera.posx, 114 - camera.posy, minifont, 4, 6, 32);
			framebuffer.drawText("this nightmare", 120 - camera.posx, 122 - camera.posy, minifont, 4, 6, 32);
		}
		else if (state == 7) {
			framebuffer.drawImage(characters.tilemap, lostSoul.position.x - camera.posx, lostSoul.position.y - camera.posy, Area(((int)(Game::instance->time * 4.f) % 4) * 16.f, 64, 16, 16));
			framebuffer.drawText("Don't follow me", 120 - camera.posx, 114 - camera.posy, minifont, 4, 6, 32);
			framebuffer.drawText("or you'll die too", 120 - camera.posx, 122 - camera.posy, minifont, 4, 6, 32);
			
		}
		else if (state == 8) {
			framebuffer.drawImage(characters.tilemap, lostSoul.position.x - camera.posx, lostSoul.position.y - camera.posy, Area(((int)(Game::instance->time * 4.f) % 4) * 16.f, 64, 16, 16));
			framebuffer.drawText("Good luck and", 120 - camera.posx, 114 - camera.posy, minifont, 4, 6, 32);
			framebuffer.drawText("goodbye forever", 120 - camera.posx, 122 - camera.posy, minifont, 4, 6, 32);
		}
		else if (state == 9) {
			framebuffer.drawImage(characters.tilemap, lostSoul.position.x - camera.posx, lostSoul.position.y - camera.posy, Area(((int)(Game::instance->time * 4.f) % 4) * 16.f, 64, 16, 16));	
		}
		else if (state == 10) {
			this->isCinePlaying = 2;
		}
		
	}
	else if (this->isCinePlaying == 0){
		framebuffer.drawImage(characters.tilemap, 142 - camera.posx, 131 - camera.posy, Area(((int)(Game::instance->time * 4.f) % 4) * 16.f, 64, 16, 16));
	}

	//Fire
	framebuffer.drawImage(tileset.tilemap, 128 - camera.posx, 240 - camera.posy, Area(70 + ((int)(Game::instance->time * 4.f) % 3) * 16.f, 16, 16, 16));
	framebuffer.drawImage(tileset.tilemap, 128 + 16 - camera.posx, 240 - camera.posy, Area(70 + ((int)(Game::instance->time * 4.f) % 3) * 16.f, 16, 16, 16));
	framebuffer.drawImage(tileset.tilemap, 128 + 32 - camera.posx, 240 - camera.posy, Area(70 + ((int)(Game::instance->time * 4.f) % 3) * 16.f, 16, 16, 16));
	


	int spritex = character.position.x - camera.posx;
	int spritey = character.position.y - camera.posy;
	spritex = clamp(spritex, -6, 148);
	spritey = clamp(spritey, -3, 107);
	int flip = 0;
	character.direction == RIGHT ? flip = 1 : flip = 0;
	framebuffer.drawImage(characters.tilemap, spritex, spritey, Area(character.inMovement ? ((int)(Game::instance->time * 4.f) % 4) * 16.f : 0, 16  * flip, 16, 16));	//draws main character


	

}

void Cave::update(double seconds_elapsed) {
	character.inMovement = false;

	

	if (character.position.x > 112 && this->isCinePlaying == 0) {
		this->isCinePlaying = 1;
	}
	else if (this->isCinePlaying != 1) {
		character.spriteUpdateMovement(seconds_elapsed, map);
	}
	if (state == 9) {
		lostSoul.position.y += 60 * seconds_elapsed;
	}
	if (character.position.x >= map->width * 16 - 13) {
		this->isFinished = true;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) //if key Z was pressed
	{

		
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_A)) //if key Z was pressed
	{
		this->isFinished = true;

	}
	if (character.position.y > 246) {
		this->isDead = true;
	}
	
}


// LAST STAGE

Last::Last() {
	this->character = Character();
	this->bgcolor = Color(0, 0, 0);
}

Last::Last(Character sprite, Color bgcolor, GameMap* map, Tileset tileset, Tileset characters) {
	this->character = sprite;
	this->character.position = Vector2(0, 163);
	this->enemy.position = Vector2(208, 112);
	this->enemy.verticalSpeed = 60;
	this->bgcolor = bgcolor;
	this->map = map;
	this->tileset = tileset;
	this->characters = characters;
	this->isFinished = false;
}

void Last::initStage() {
	this->isDead = false;
	this->isFinished = false;
	this->character.hasCoin = false;
	this->character.position = Vector2(0, 163);
	this->enemy.position = Vector2(208, 112);
	this->enemy.verticalSpeed = 100;
	time_limit = getTime() + 1 * 1000;
}

void Last::render(Image& framebuffer) {
	// Size in pixels of a cell
	int cs = tileset.tileset_width / tileset.numtiles;


	camera.updateCamera(framebuffer.width, framebuffer.height, character.position, map->width, map->height, cs);

	framebuffer.fill(bgcolor);
	map->drawMap(framebuffer, tileset, camera.posx, camera.posy, cs);

	if (character.hasCoin == false) {
		this->coin.position.x = 64 - camera.posx;
		this->coin.position.y = ((((int)(Game::instance->time * 1.5) % 2) == 1) ? 63 : 64) - camera.posy;
		framebuffer.drawImage(tileset.tilemap, coin.position.x, coin.position.y, Area((((int)(Game::instance->time * 4.f) % 6) * 16.f), 96, 16, 16));
	}
	//ENEMY
	int enemyx = enemy.position.x - camera.posx;
	int enemyy = enemy.position.y - camera.posy;
	int animation = 0;
	
	if (enemy.position.y < 96) {
		animation = 2;
	}
	else if (enemy.position.y < 104) {
		animation = 1;
	}
	framebuffer.drawImage(characters.tilemap, enemyx, enemyy, Area(16 * animation, 96, 16, 16));


	if (character.position.x + 8 < 480 && character.position.x + 8 > 448 && character.position.y + 8 > 32 && character.position.y + 8 < 64 && character.hasCoin == false) {
		framebuffer.drawText("You need the coin", 420 - camera.posx, 16 - camera.posy, minifont, 4, 6, 32);
	}


	

	int spritex = character.position.x - camera.posx;
	int spritey = character.position.y - camera.posy;
	spritex = clamp(spritex, -6, 148);
	spritey = clamp(spritey, -3, 107);
	int flip = 0;
	character.direction == RIGHT ? flip = 1 : flip = 0;
	framebuffer.drawImage(characters.tilemap, spritex, spritey, Area(character.inMovement ? ((int)(Game::instance->time * 6.f) % 4) * 16.f : 0, 16 * flip, 16, 16));	//draws only a part of an image	
	



	

}

void Last::update(double seconds_elapsed) {
	character.inMovement = false;

	character.spriteUpdateMovement(seconds_elapsed, map);

	//Enemy
	if (enemy.position.y <= 30) {
		enemy.verticalSpeed = -100;
		
	}
	else if (enemy.position.y >= 112 && state == 0) {
		enemy.verticalSpeed = 100;
	}

	if (enemy.position.y >= 112 && state == 0) {
		state = 1;
		time_limit = getTime() + 0.5 * 1000.f;
	}


	if (time_limit < getTime()) {
		enemy.position.y -= enemy.verticalSpeed * seconds_elapsed;
		enemy.position.y = clamp(enemy.position.y, 30, 112);
		state = 0;
	}
	
	if (character.position.x + 8 > enemy.position.x && character.position.x + 8 < enemy.position.x + 16 && character.position.y + 8 > enemy.position.y + 6 && character.position.y + 8 < enemy.position.y + 20) {
		this->isDead = true;
	}
	
	

	if (character.position.x + 8 > coin.position.x && character.position.x + 8 < coin.position.x + 16 && character.position.y + 8 > coin.position.y && character.position.y + 8 < coin.position.y + 16) {
		if (character.hasCoin == false) {
			Game::instance->synth.playSample(pickupSound, 1, false);
		}
		character.hasCoin = true;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_A)) //if key Z was pressed
	{
		this->isFinished = true;

	}

	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) //if key Z was pressed
	{
		if (character.position.x + 8 < 480 && character.position.x + 8 > 448 && character.position.y + 8 > 32 && character.position.y+ 8 < 64 && character.hasCoin == true) {
			character.hasCoin = false;
			this->isFinished = true;
		}
	}


	
}


// DEATH STAGE
Death::Death() {
	this->isDead = false;
	this->isFinished = false;
	this->state = 0;

	death = new Image((5 * 8 + 2 * 8), 9);
	death->drawText("YOU DIED", 0, 0, font);

	cont = new Image((5 * 16 + 2 * 16), 9);
	cont->drawText("Press Z to retry", 0, 0, font);;
}


void Death::initStage() {
	this->isDead = false;
	this->isFinished = false;
	this->state = 0;

	death = new Image((5 * 8 + 2 * 8), 9);
	death->drawText("YOU DIED", 0, 0, font);

	cont = new Image((5 * 16 + 2 * 16), 9);
	cont->drawText("Press Z to retry", 0, 0, font);;
}

void Death::render(Image& framebuffer){
	death->changeColor(Color(255, 50, 50));
	cont->changeColor(Color(50, 50, 50));


	int shake = 1;

	cont->changeColor(Color(150, 70, 70));

	framebuffer.drawImage(*death, (framebuffer.width / 2) - (5 * 8 + 2 * 8) / 2, 20);
	framebuffer.drawImage(*cont, (framebuffer.width / 2) - (5 * 16 + 2 * 16) / 2, ((int)(Game::instance->time * 1.5) % 2) + 100);

}

void Death::update(double seconds_elapsed) {
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) //if key Z was pressed
	{
		this->isFinished = true;

	}
}

// END STAGE
End::End() {
	this->isDead = false;
	this->isFinished = false;
}

void End::initStage() {
	this->isDead = false;
	this->isFinished = false;
}

void End::render(Image& framebuffer) {
	framebuffer.drawText("You woke up and", (framebuffer.width / 2) - (5 * 15 + 2 * 15) / 2, 20, font);
	framebuffer.drawText("escaped", (framebuffer.width / 2) - (5 * 7 + 2 * 7) / 2, 35, font);
	
	framebuffer.drawText("The End", (framebuffer.width / 2) - (5 * 8 + 2 * 8) / 2, 100, font);
}

void End::update(double seconds_elapsed) {
	
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) //if key Z was pressed
	{

		this->isFinished = true;

	}
	if (Input::isKeyPressed(SDL_SCANCODE_UP)) //if key up
	{

	}
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key up
	{

	}
}