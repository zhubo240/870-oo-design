/*
 * Enemy.cpp
 *
 *  Created on: Nov 26, 2015
 *      Author: zhubo
 */

#include "SmartEnemy.h"
#include "gamedata.h"
#include "ExplodingMultiSprite.h"dd

SmartEnemy::SmartEnemy(const string& name, const Drawable* target) :
		MultiSprite(name), target(target), aimDis(
				Gamedata::getInstance().getXmlInt(name + "/aimDis")), isVisiable(true) {

}

void SmartEnemy::init(const Vector2f pos){
	this->setPosition(pos);
	this->isVisiable = true;
}

bool SmartEnemy::getIsVisiable(){
	return this->isVisiable;
}


void SmartEnemy::update(Uint32 ticks) {
	if (explosion) {
//		std::cout << "multisprite update" << std::endl;
		explosion->update(ticks);
		if (explosion->chunkCount() == 0) {
			delete explosion;
			explosion = NULL;
			this->isVisiable = false;
		}
		return;
	}

	advanceFrame(ticks);
	move(ticks);
}


void SmartEnemy::setIsVisiable(bool v){
	this->isVisiable = v;
}

//This move strategy
// distance to aim
//
void SmartEnemy::move(Uint32 ticks) {
	int dis = this->getDistance(this->target);
	if (this->X() > this->target->X()+ target->getFrame()->getWidth()/2) {
		if (dis <= this->aimDis) {
			//change dir
			int v = this->getVelocity().magnitude();
			Vector2f diffPos = this->target->getPosition()
					+ Vector2f(target->getFrame()->getWidth()/2, target->getFrame()->getHeight()/2)
					- this->getPosition();

			float ratio = v / diffPos.magnitude();
//			std::cout << diffPos[0] << ", " << diffPos[0] << ", " << ratio << std::endl;
			this->setVelocity(ratio * diffPos);
		}
	}

	if (this->X() <= this->target->X() + target->getFrame()->getWidth()/2) {
		int v = this->getVelocity().magnitude();
		this->setVelocity(Vector2f(-1*v, 0));
	}

	Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
	setPosition(getPosition() + incr);

	if (Y() < 0) {
		velocityY(abs(velocityY()));
	}
	if (Y() > worldHeight - frameHeight) {
		velocityY(-abs(velocityY()));
	}

	if (X() < 0) {
		velocityX(abs(velocityX()));
	}
	if (X() > worldWidth - frameWidth) {
		velocityX(-abs(velocityX()));
	}
}

SmartEnemy::~SmartEnemy() {
	// TODO Auto-generated destructor stub
}

