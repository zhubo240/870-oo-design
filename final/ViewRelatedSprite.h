/*
 * ViewRelatedSprite.h
 *
 *  Created on: Nov 11, 2015
 *      Author: zhubo
 */

#ifndef VIEWRELATEDSPRITE_H_
#define VIEWRELATEDSPRITE_H_

#include <SDL/SDL_stdinc.h>
#include <string>

#include "sprite.h"

class Viewport;

class ViewRelatedSprite: public Sprite {
public:
	ViewRelatedSprite(const std::string& name);
	virtual void move(Uint32 ticks);
	virtual ~ViewRelatedSprite();

protected:
	Viewport& view;
};

#endif /* VIEWRELATEDSPRITE_H_ */
