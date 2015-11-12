/*
 * Player.h
 *
 *  Created on: Nov 11, 2015
 *      Author: zhubo
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "TwoWaySprite.h"

class Player: public TwoWaySprite {
public:
	Player(const string& name);
	//Player(const Player& player);
	virtual ~Player();

	//supper class
//	virtual void update(Uint32 ticks);
//	virtual void draw() const;

	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();
	void right();
	void left();
	void up();
	void down();

protected:
//	float frameInterval;
//	float minFrameDelay;
//	float frameDelay;

	bool isLeftKeyPressed;
	bool isRightKeyPressed;
	bool isUpKeyPressed;
	bool isDownKeyPressed;

	const int speedX;
	const int speedY;
};

#endif /* PLAYER_H_ */
