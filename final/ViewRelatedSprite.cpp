/*
 * ViewRelatedSprite.cpp
 *
 *  Created on: Nov 11, 2015
 *      Author: zhubo
 */

#include "ViewRelatedSprite.h"

#include <cstdlib>

#include "drawable.h"
#include "vector2f.h"
#include "viewport.h"

ViewRelatedSprite::ViewRelatedSprite(const std::string & name) :
		Sprite(name), view(Viewport::getInstance()) {
	//TODO : do not need to know about the v.
	//But a sprite need to know the v.
//	this->setVelocity(view.getObjectToTrack()->getVelocity()) * this->getZoom();
}

ViewRelatedSprite::~ViewRelatedSprite() {
}

void ViewRelatedSprite::move(Uint32 ticks) {
	//TODO : not good
	Vector2f incr = Vector2f(view.getObjectToTrack()->getVelocity()[0]
			* static_cast<float>(ticks) * 0.001 * (1 - this->getZoom()), 0);

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
