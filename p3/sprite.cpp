#include "sprite.h"

#include <SDL/SDL_stdinc.h>
#include <cmath>
#include <cstdlib>

#include "frame.h"
#include "frameFactory.h"
#include "gamedata.h"
#include "vector2f.h"

//Sprite::Sprite(const string& n, const Vector2f& pos, const Vector2f& vel,
//		const Frame* frm) :
//		Drawable(n, pos, vel, Gamedata::getInstance().getRandFloat(Gamedata::getInstance().getXmlFloat(name + "/zoom/min"),
//				Gamedata::getInstance().getXmlFloat(name + "/zoom/max"))),
//				frame(frm), frameWidth(frame->getWidth()),
//		frameHeight(frame->getHeight()), worldWidth(Gamedata::getInstance().getXmlInt("world/width")), worldHeight(
//				Gamedata::getInstance().getXmlInt("world/height"))
//				  {
//}

Sprite::Sprite(const std::string& name) :
		Drawable(name,
		           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
		                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
		           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
		                    Gamedata::getInstance().getXmlInt(name+"/speedY")),
							Gamedata::getInstance().getRandFloat(Gamedata::getInstance().getXmlFloat(name + "/zoom/min"),
										Gamedata::getInstance().getXmlFloat(name + "/zoom/max")) ),

		 frame(FrameFactory::getInstance().getFrame(name)),
		 //TODO: I change this
		 frameWidth(FrameFactory::getInstance().getFrame(name)->getWidth() * this->getZoom()),
				frameHeight(FrameFactory::getInstance().getFrame(name)->getHeight() * this->getZoom()),
				worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
				worldHeight(Gamedata::getInstance().getXmlInt("world/height")){
}

Sprite::Sprite(const Sprite& s) :
		Drawable(s), frame(s.frame), frameWidth(s.getFrame()->getWidth()), frameHeight(
				s.getFrame()->getHeight()), worldWidth(
				Gamedata::getInstance().getXmlInt("world/width")), worldHeight(
				Gamedata::getInstance().getXmlInt("world/height")) {
}

Sprite& Sprite::operator=(const Sprite& rhs) {
	Drawable::operator=(rhs);
	frame = rhs.frame;
	frameWidth = rhs.frameWidth;
	frameHeight = rhs.frameHeight;
	worldWidth = rhs.worldWidth;
	worldHeight = rhs.worldHeight;
	return *this;
}

void Sprite::draw() const {
	Uint32 x = static_cast<Uint32>(X());
	Uint32 y = static_cast<Uint32>(Y());
	//TODO: you need to consider the zoom
	frame->draw(x, y, 0., this->getZoom(), 0);
}


int Sprite::getDistance(const Sprite *obj) const {
	return hypot(X() - obj->X(), Y() - obj->Y());
}

void Sprite::update(Uint32 ticks) {
	move(ticks);
}

void Sprite::move(Uint32 ticks) {
	Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001 * this->getZoom();

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
