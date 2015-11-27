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

	bool getIsVisiable();
	void setIsVisiable(bool v);
	void init(const Vector2f v);

protected:
	virtual void move(Uint32 ticks);
	const Drawable* const target;
	const int aimDis;

	bool isVisiable;
};

#endif /* SMARTENEMY_H_ */
