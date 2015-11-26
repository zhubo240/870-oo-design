
#include "TwoWayMultiSprite.h"

#include <SDL/SDL_stdinc.h>
#include <cstdlib>
#include <string>

#include "drawable.h"
#include "gamedata.h"
#include "vector2f.h"

TwoWayMultiSprite::TwoWayMultiSprite(const string& name) :
		MultiSprite(name), dir(LARGER), numFramesOneDir(Gamedata::getInstance().getXmlInt(name+"/numFramesOneDir")), dirChanged(
				false) {
	if(this->X() >= 0) dir = LARGER;
	else dir = SMALLER;
}

TwoWayMultiSprite::~TwoWayMultiSprite() {
	//do nothing
}


void TwoWayMultiSprite::advanceFrame(Uint32 ticks) {
	if (this->dirChanged) {
		if (this->dir == LARGER)
			this->dir = SMALLER;
		else
			this->dir = LARGER;

		this->currentFrame = 0;
		this->dirChanged = false;
	} else {
		timeSinceLastFrame += ticks;

		if (timeSinceLastFrame > frameInterval) {
			if (this->dir == LARGER)
				currentFrame = (currentFrame + 1) % this->numFramesOneDir;
			if (this->dir == SMALLER)
				currentFrame = (currentFrame + 1) % this->numFramesOneDir
						+ this->numFramesOneDir;
			timeSinceLastFrame = 0;
		}

	}
}

void TwoWayMultiSprite::move(Uint32 ticks) {
	Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
	setPosition(getPosition() + incr);

	if (Y() < 0) {
		velocityY(abs(velocityY()));
		velocityY(0);
	}
	if (Y() > worldHeight - frameHeight) {
		velocityY(-abs(velocityY()));
		velocityY(0);
	}

	if (X() < 0) {
		velocityX(abs(velocityX()));
		this->X(0);
		this->dirChanged = true;

	}

	if (X() > worldWidth - frameWidth) {
		velocityX(-abs(velocityX()));
		this->X(worldWidth - frameWidth);
		this->dirChanged = true;
	}
}

