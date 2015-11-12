/*
 * Player.cpp
 *
 *  Created on: Nov 11, 2015
 *      Author: zhubo
 */

#include "Player.h"

#include <string>

#include "drawable.h"
#include "gamedata.h"
#include "vector2f.h"


Player::Player(const string& name) : TwoWayMultiSprite(name), isUpKeyPressed(false), isDownKeyPressed(false),
			isLeftKeyPressed(false), isRightKeyPressed(false), speedX(Gamedata::getInstance().getXmlInt("player/speedX")),
			speedY(Gamedata::getInstance().getXmlInt("player/speedY")){

}
//
//Player::Player(const Player& player) {
//}

Player::~Player() {
	// TODO Auto-generated destructor stub
}

void Player::stopX() {

	Vector2f v = this->getVelocity();
	v[0] = 0;

	this->setVelocity(v);
}


void Player::stopY() {
	Vector2f v = this->getVelocity();
	v[1] = 0;

	this->setVelocity(v);
}


void Player::right() {
	Vector2f v = this->getVelocity();
	v[0] = this->speedX;

	this->setVelocity(v);
}

void Player::left() {
	Vector2f v = this->getVelocity();
	v[0] = (-1) * this->speedX;

	this->setVelocity(v);
}

void Player::up() {
	Vector2f v = this->getVelocity();
	v[1] = (-1) * this->speedY;

	this->setVelocity(v);
}

void Player::down() {
	Vector2f v = this->getVelocity();
	v[1] = this->speedY;

	this->setVelocity(v);
}
