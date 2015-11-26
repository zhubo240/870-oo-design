/*
 * Enemy.cpp
 *
 *  Created on: Nov 26, 2015
 *      Author: zhubo
 */

#include "SmartEnemy.h"
#include "gamedata.h"

SmartEnemy::SmartEnemy(const string& name, const Drawable* target) :
		MultiSprite(name), target(target), aimDis(
				Gamedata::getInstance().getXmlInt(name + "/aimDis")) {

}
//This move strategy
// distance to aim
//
void SmartEnemy::move(Uint32 ticks) {
	int dis = this->getDistance(this->target);
	if (this->X() > this->target->X()) {
		if (dis <= this->aimDis) {
			//change dir
			int v = this->getVelocity().magnitude();
			Vector2f diffPos = this->target->getPosition()
					- this->getPosition();
			float ratio = v / diffPos.magnitude();
			this->setVelocity(ratio * diffPos);
		}
	}

	if (this->X() <= this->target->X()) {
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

