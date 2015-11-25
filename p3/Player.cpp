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


Player::Player(const string& name) : TwoWayMultiSprite(name), speedX(Gamedata::getInstance().getXmlInt("player/speedX")),
			speedY(Gamedata::getInstance().getXmlInt("player/speedY")){

}


//
//Player::Player(const Player& player) {
//}

Player::~Player() {
	// TODO Auto-generated destructor stub
}

void Player::move(Uint32 ticks) {
	//check explode
	Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
	setPosition(getPosition() + incr);

	if (Y() < 0) {
		velocityY(0);
	}
	if (Y() > worldHeight - frameHeight) {
		velocityY(0);
	}

	if (X() < 0) {
		velocityX(abs(velocityX()));
		this->X(0);
		//this->dirChanged = true;

	}
	if (X() > worldWidth - frameWidth) {
		velocityX(-abs(velocityX()));
		this->X(worldWidth - frameWidth);
		//this->dirChanged = true;
	}
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
	this->dir = LARGER;
	Vector2f v = this->getVelocity();
	v[0] = this->speedX;

	this->setVelocity(v);
}

void Player::left() {
	this->dir = SMALLER;
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

void Player::shoot() const {
	Vector2f v = this->getVelocity();
	Vector2f pos = this->getPosition();
	//player is a two way mutiple sprite
	if(this->dir == LARGER)
		BulletPool::getInstance().createBullet(pos[0] + this->getFrame()->getWidth(), pos[1], v[0] + 130, 0);
	else
		BulletPool::getInstance().createBullet(pos[0] , pos[1], v[0] - 130, 0);
}

void Player::blow() {
	this->explode();
}
