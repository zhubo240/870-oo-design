/*
 * Hud.cpp
 *
 *  Created on: Nov 6, 2015
 *      Author: zhubo
 */

#include "Hud.h"

#include <SDL/SDL_video.h>

#include "aaline.h"
#include "clock.h"
#include "gamedata.h"
#include "ioManager.h"

Hud::Hud(const string& name) :
		screen(IOManager::getInstance().getScreen()),
		clock(Clock::getInstance()),
		x(Gamedata::getInstance().getXmlInt(name + "/position/x")), y(
				Gamedata::getInstance().getXmlInt(name + "/position/y")), width(
				Gamedata::getInstance().getXmlInt(name + "/width")), height(
				Gamedata::getInstance().getXmlInt(name + "/height")), alpha(
		//TODO : why divide 2
				Gamedata::getInstance().getXmlInt(name + "/alpha")), thickness(
				this->height * 2), rectColors(vector<int>(3)), bordColors(
				vector<int>(3)), maxTime(
				Gamedata::getInstance().getXmlInt("hud/maxTime")), leftTime(
				Gamedata::getInstance().getXmlInt("hud/maxTime")), isVisiable(true) {
	this->rectColors[0] = Gamedata::getInstance().getXmlInt(name + "/rect/red");
	this->rectColors[1] = Gamedata::getInstance().getXmlInt(
			name + "/rect/yellow");
	this->rectColors[2] = Gamedata::getInstance().getXmlInt(
			name + "/rect/blue");
	this->rectColor = SDL_MapRGB(screen->format, this->rectColors[0],
			this->rectColors[1], this->rectColors[2]);

	this->bordColors[0] = Gamedata::getInstance().getXmlInt(name + "/bord/red");
	this->bordColors[1] = Gamedata::getInstance().getXmlInt(
			name + "/bord/yellow");
	this->bordColors[2] = Gamedata::getInstance().getXmlInt(
			name + "/bord/blue");
	this->bordColor = SDL_MapRGB(screen->format, this->bordColors[0],
			this->bordColors[1], this->bordColors[2]);
}

void Hud::draw() const {
	//TODO : for sdl, there is only have function to draw picture or pixel
	//draw rect
	drawRect();
	//draw bord
	drawBord();
	//msg
	drawMsg();
}

void Hud::drawRect() const {
	Draw_AALine(this->screen, this->x, this->y, this->x + this->width, this->y,
			this->thickness, this->rectColor);
}

void Hud::drawBord() const {
	// Two Horizontal lines
	Draw_AALine(this->screen, x, y, x + width, y, 3, this->bordColor);

	Draw_AALine(this->screen, x, y + height, x + width, y + height, 3,
			this->bordColor);

	// Two Vertical lines
	Draw_AALine(this->screen, x, y, x, y + height, 3, this->bordColor);
	Draw_AALine(screen, x + width, y, x + width, y + height, 3,
			this->bordColor);
}

void Hud::drawMsg() const {
	clock.draw();
}


void Hud::update(int ticks) {
	if(this->isVisiable){
	this->leftTime -= ticks;

	if(this->leftTime <= 0){
		this->isVisiable = false;
		this->leftTime = this->maxTime;
	}
	}
}
Hud::~Hud() {
	// TODO Auto-generated destructor stub
}
