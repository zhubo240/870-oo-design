/*
 * Bullet.h
 *
 *  Created on: Nov 19, 2015
 *      Author: zhubo
 */

#ifndef BULLET_H_
#define BULLET_H_

#include "sprite.h"
#include "ioManager.h"
#include "SDL/SDL.h"
#include "aaline.h"
#include "viewport.h"

class Bullet: public Drawable {
public:
	Bullet(const std::string name, const Vector2f& pos, const Vector2f& vel,
			float zoom);
	Bullet(const Bullet&);
	Bullet& operator=(const Bullet& bullet);
	virtual ~Bullet();

	virtual const Frame* getFrame() const;
	virtual void draw() const;
	virtual void update(Uint32 ticks);

	void init(const Vector2f& pos, const Vector2f& vel);
	void setIsVisiable(bool v);
	bool getIsVisiable();

	virtual bool collidedWith(const Drawable*) const;

protected:
	int worldWidth;
	int worldHeight;
	bool isVisiable;

	SDL_Surface* screen;

	std::vector<CollisionStrategy*> strategies;
	CollisionStrategy * strategy;
	void initCollision();
};

#endif /* BULLET_H_ */
