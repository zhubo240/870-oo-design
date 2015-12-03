/*
 * Bullet.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: zhubo
 */

#include "Bullet.h"
#include "frameFactory.h"

Bullet::Bullet(const std::string name, const Vector2f& pos, const Vector2f& vel,
		float zoom) :
		Drawable(name, pos, vel, zoom), worldWidth(
				Gamedata::getInstance().getXmlInt("world/width")), worldHeight(
				Gamedata::getInstance().getXmlInt("world/height")), isVisiable(
				true), screen(IOManager::getInstance().getScreen()) {
	initCollision();
}

Bullet::Bullet(const Bullet& bullet) : Drawable(bullet), worldWidth(
		bullet.worldWidth), worldHeight(bullet.worldHeight), isVisiable(
		bullet.isVisiable), screen(bullet.screen){

}

const Frame* Bullet::getFrame() const {
	Frame* frame = FrameFactory::getInstance().getFrame("food");
	frame->setHeight(20);
	frame->setWidth(20);
	return frame;
}

Bullet& Bullet::operator=(const Bullet& bullet){
	//TODO: how to write it ?
	if(this == &bullet) return *this;

	Drawable::operator=(bullet);
	this->worldWidth = bullet.worldHeight;
	this->worldHeight = bullet.worldHeight;
	this->isVisiable = bullet.isVisiable;
	this->screen = bullet.screen;

	return *this;
}

Bullet::~Bullet() {
}

void Bullet::draw() const {
	//std::cout << "draw -- "  << this->getPosition()[0] << ", " << this->getPosition()[1]<< std::endl;
	Vector2f pos = this->getPosition() - Viewport::getInstance().getPosition();

	Draw_AALine(screen, pos[0], pos[1],
			pos[0], pos[1] + 20, 20,
			SDL_MapRGB(screen->format, 255, 255, 255));
}

void Bullet::update(Uint32 ticks) {
	Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;

	setPosition(getPosition() + incr);

	if (Y() < 0 || Y() > worldHeight || X() < 0 || X() > worldWidth) {
		this->isVisiable = false;
	}
}

void Bullet::init(const Vector2f& pos, const Vector2f& vel) {
	this->setPosition(pos);
	this->setVelocity(vel);

	this->isVisiable = true;
}

void Bullet::setIsVisiable(bool v) {
	this->isVisiable = v;
}

bool Bullet::getIsVisiable() {
	return this->isVisiable;
}

bool Bullet::collidedWith(const Drawable* d) const {
  return strategy->execute(*this, *d);
}


void Bullet::initCollision() {
		  this->strategies.push_back(new RectangularCollisionStrategy());
		  this->strategies.push_back(new MidPointCollisionStrategy());
		  this->strategies.push_back(new PerPixelCollisionStrategy());
		  this->strategy = this->strategies[0];
}
