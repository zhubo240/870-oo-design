/*
 * Player.h
 *
 *  Created on: Nov 11, 2015
 *      Author: zhubo
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "multisprite.h"
#include "BulletPool.h"

class Player: public MultiSprite {
public:
	Player(const string& name);
	//Player(const Player& player);
	virtual ~Player();

	//supper class
	virtual void move(Uint32 ticks);
//	virtual void update(Uint32 ticks);
//	virtual void draw() const;

	void stopX();
	void stopY();
	void right();
	void left();
	void up();
	void down();

	void shoot() const;
	void blow();
protected:

	const int speedX;
	const int speedY;
};

#endif /* PLAYER_H_ */
