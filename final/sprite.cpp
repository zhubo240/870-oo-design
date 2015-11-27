#include "sprite.h"

//#include <SDL/SDL_stdinc.h>
//#include <cmath>
//#include <cstdlib>
//
//#include "frame.h"
//#include "frameFactory.h"
//#include "gamedata.h"
//#include "vector2f.h"


//TODO:
//it change to this right
#include <cmath>
#include "sprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "ExplodingSprite.h"


Sprite::Sprite(const std::string& name) :
		Drawable(name,
		           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
		                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
		           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
		                    Gamedata::getInstance().getXmlInt(name+"/speedY")),
							Gamedata::getInstance().getRandFloat(Gamedata::getInstance().getXmlFloat(name + "/zoom/min"),
										Gamedata::getInstance().getXmlFloat(name + "/zoom/max")) ),
										explosion(NULL),
		 frame(FrameFactory::getInstance().getFrame(name)),
		 //TODO: I change this
		 frameWidth(FrameFactory::getInstance().getFrame(name)->getWidth() * this->getZoom()),
				frameHeight(FrameFactory::getInstance().getFrame(name)->getHeight() * this->getZoom()),
				worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
				worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
				strategies(), strategy( NULL ){
	initCollision();
}

Sprite::Sprite(const Sprite& s) :
		Drawable(s), explosion(NULL), frame(s.frame), frameWidth(s.getFrame()->getWidth()), frameHeight(
				s.getFrame()->getHeight()), worldWidth(
				Gamedata::getInstance().getXmlInt("world/width")), worldHeight(
				Gamedata::getInstance().getXmlInt("world/height")), strategies(), strategy(NULL){
	initCollision();
}

//TODO: this kind of constructor, for chucks
Sprite::Sprite(const string& name, const Vector2f& pos, const Vector2f& vel,
		const Frame* frm) :
		Drawable(name, pos, vel, Gamedata::getInstance().getRandFloat(Gamedata::getInstance().getXmlFloat(name + "/zoom/min"),
				Gamedata::getInstance().getXmlFloat(name + "/zoom/max"))),
				explosion(NULL), frame(frm), frameWidth(
				frame->getWidth()), frameHeight(frame->getHeight()), worldWidth(
				Gamedata::getInstance().getXmlInt("world/width")), worldHeight(
				Gamedata::getInstance().getXmlInt("world/height")), strategies(), strategy( NULL ) {
	initCollision();
}

Sprite::~Sprite(){
	delete this->explosion;

	for(unsigned i = 0; i < this->strategies.size(); i++)
		delete this->strategies[i];
}

Sprite& Sprite::operator=(const Sprite& rhs) {
	Drawable::operator=(rhs);
	explosion = rhs.explosion;
	frame = rhs.frame;
	frameWidth = rhs.frameWidth;
	frameHeight = rhs.frameHeight;
	worldWidth = rhs.worldWidth;
	worldHeight = rhs.worldHeight;

	//TODO;
	for(unsigned i = 0; i < this->strategies.size(); i++){
		*this->strategies[i] = *rhs.strategies[i];
	}
	*this->strategy = *rhs.strategy;

	return *this;
}


void Sprite::draw() const {
	if (explosion) {
		explosion->draw();
		return;
	}

	Uint32 x = static_cast<Uint32>(X());
	Uint32 y = static_cast<Uint32>(Y());
	//TODO: you need to consider the zoom
//	frame->draw(frame->getSourceX(),frame->getSourceY(), x, y, 0., this->getZoom(), 1);
	frame->draw(x, y);
}

bool Sprite::explode() {
	if (explosion)
		return false;

	explosion = new ExplodingSprite(*this);
	return true;
}


int Sprite::getDistance(const Sprite *obj) const {
	return hypot(X() - obj->X(), Y() - obj->Y());
}

void Sprite::update(Uint32 ticks) {
	if (explosion) {
			explosion->update(ticks);
			if (explosion->chunkCount() == 0) {
				delete explosion;
				explosion = NULL;
			}
			return;
		}
	move(ticks);
}

void Sprite::move(Uint32 ticks) {

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

bool Sprite::collidedWith(const Drawable* d) const {
  return strategy->execute(*this, *d);
}

void Sprite::initCollision() {
		  this->strategies.push_back(new RectangularCollisionStrategy());
		  this->strategies.push_back(new MidPointCollisionStrategy());
		  this->strategies.push_back(new PerPixelCollisionStrategy());
		  this->strategy = this->strategies[2];
}

