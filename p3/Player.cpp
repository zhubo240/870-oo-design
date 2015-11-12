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


Player::Player(const string& name) : TwoWaySprite(name), isUpKeyPressed(false), isDownKeyPressed(false),
			isLeftKeyPressed(false), isRightKeyPressed(false), speedX(Gamedata::getInstance().getXmlInt("player/speedX")),
			speedY(Gamedata::getInstance().getXmlInt("player/speedY")){

}
//
//Player::Player(const Player& player) {
//}

Player::~Player() {
	// TODO Auto-generated destructor stub
}

void Player::stopLeft() {

	Vector2f oldv = this->getVelocity();
	oldv[0] += this->speedX;

	this->setVelocity(oldv);
}

void Player::stopRight() {
	Vector2f oldv = this->getVelocity();
	oldv[0] -= this->speedX;

	this->setVelocity(oldv);

}

void Player::stopUp() {
	Vector2f oldv = this->getVelocity();
	oldv[1] += this->speedY;

	this->setVelocity(oldv);
}

void Player::stopDown() {
	Vector2f oldv = this->getVelocity();
	oldv[1] -= this->speedY;

	this->setVelocity(oldv);

}

void Player::right() {
	this->stopLeft();
}

void Player::left() {
	this->stopRight();
}

void Player::up() {
	this->stopDown();
}

void Player::down() {
	this->stopUp();
}
