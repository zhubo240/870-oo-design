/*
 * BulletPool.h
 *
 *  Created on: Nov 19, 2015
 *      Author: zhubo
 */

#ifndef BULLETPOOL_H_
#define BULLETPOOL_H_

#include <SDL/SDL_stdinc.h>
#include <list>
#include <sstream>

#include "Bullet.h"
#include "ioManager.h"


class BulletPool {
public:
	virtual ~BulletPool();

	static BulletPool& getInstance();
	void draw();
	void update(Uint32 ticks);
	void createBullet(int x, int y, int vx, int vy);
	void reset();
	void drawMsg();
private:
	BulletPool();

	std::list<Bullet*> bullets;
	std::list<Bullet*> freeList;
};

#endif /* BULLETPOOL_H_ */
