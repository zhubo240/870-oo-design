/*
 * TwoWaySprite.h
 *
 *  Created on: Oct 13, 2015
 *      Author: zhubo
 */

#ifndef TWOWAYSPRITE_H_
#define TWOWAYSPRITE_H_

#include "multisprite.h"

class TwoWaySprite: public MultiSprite {
public:
	TwoWaySprite(const string & name);
	virtual ~TwoWaySprite();

//	 TODO : even I know it is virtual, I still need to write those there
	  //why there are some link error, if I keep this virtual method in .h
	//virtual void draw() const;
	virtual void update(Uint32 ticks);
	virtual void advanceFrame(Uint32 ticks);
	virtual void move(Uint32 ticks);

protected:
	enum Dir {LARGER, SMALLER};
	//TODO : enum Dir or Dir
	Dir dir;

	//enum
	int numFramesOneDir;
	bool dirChanged;
};

#endif /* TWOWAYSPRITE_H_ */
