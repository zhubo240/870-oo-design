/*
 * EnemyPool.h
 *
 *  Created on: Nov 26, 2015
 *      Author: zhubo
 */

#ifndef ENEMYPOOL_H_
#define ENEMYPOOL_H_

#include "SmartEnemy.h"
#include "viewport.h"
#include "drawable.h"
#include <list>

class EnemyPool {
public:
	EnemyPool(const Drawable* player);
	virtual ~EnemyPool(){};

	static EnemyPool& getInstance(const Drawable* player);
	void draw();
	void update(Uint32 ticks);
	void createEnemy();
	void reset();
	void drawMsg();
	std::list<SmartEnemy*> getEnemies(){
		return this->enemies;
	}

	bool isVisiable;

private:
	EnemyPool(const Drawable& player);

	std::list<SmartEnemy*> enemies;
	std::list<SmartEnemy*> freeList;


	const Drawable* const player;
	const Viewport& viewport;
};

#endif /* ENEMYPOOL_H_ */
