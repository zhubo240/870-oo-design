/*
 * Enemy.h
 *
 *  Created on: Nov 26, 2015
 *      Author: zhubo
 */

#ifndef SMARTENEMY_H_
#define SMARTENEMY_H_

#include "multisprite.h"

class SmartEnemy: public MultiSprite {
public:
	SmartEnemy(const string& name, const Drawable* target);
	virtual ~SmartEnemy();

protected:
	virtual void move(Uint32 ticks);
	const Drawable* const target;
	const int aimDis;
};

#endif /* SMARTENEMY_H_ */
