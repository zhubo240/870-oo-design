/*
 * BulletPool.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: zhubo
 */

#include "BulletPool.h"

BulletPool::BulletPool() : isVisiable(false), bullets(), freeList(){}

BulletPool::~BulletPool(){
	//bullets
	for (std::list<Bullet*>::iterator iter = this->bullets.begin();
			iter != this->bullets.end(); iter++) {
		delete *iter;
	}

	//freeList
	for (std::list<Bullet*>::iterator iter = this->freeList.begin();
			iter != this->freeList.end(); iter++) {
		delete *iter;
	}
}

void BulletPool::reset(){
	this->isVisiable = true;
	for (std::list<Bullet*>::iterator iter = this->bullets.begin();
				iter != this->bullets.end(); iter++) {
			delete *iter;
		}

		//freeList
		for (std::list<Bullet*>::iterator iter = this->freeList.begin();
				iter != this->freeList.end(); iter++) {
			delete *iter;
		}

	this->bullets.resize(0);
	this->freeList.resize(0);
}

BulletPool& BulletPool::getInstance()
		{

	static BulletPool pool;
	return pool;
}

void BulletPool::draw() {
	for (std::list<Bullet*>::iterator iter = this->bullets.begin();
			iter != this->bullets.end(); iter++) {
		(*iter)->draw();
	}
}

void BulletPool::drawMsg(){
	if(this->isVisiable){
	//freeList
	int x  = 70, y = 200;
	std::stringstream s;
	s << this->freeList.size();
	IOManager::getInstance().printMessageAt("freelist size = " + s.str(), x, y);

	int i = 1;
	for (std::list<Bullet*>::iterator iter = this->bullets.begin();
				iter != this->bullets.end(); iter++, i++) {
		std::stringstream ss;
		ss << "bullet" << i << " : " << "pos : " << (*iter)->getPosition()[0] << ", " << (*iter)->getPosition()[1]
												<< ", v :"<< (*iter)->getVelocity()[0] << ", " << (*iter)->getVelocity()[1];
		IOManager::getInstance().printMessageAt(ss.str(), x, i * 15 + y);
		}
	}

}

void BulletPool::update(Uint32 ticks) {
	//bullets
	for (std::list<Bullet*>::iterator iter = this->bullets.begin();
			iter != this->bullets.end(); iter++) {
		(*iter)->update(ticks);
	}

	//move bullet
	for (std::list<Bullet*>::iterator iter = this->bullets.begin();
			iter != this->bullets.end();) {
		if ((*iter)->getIsVisiable() == false) {
			this->freeList.push_back(*iter);
			iter = this->bullets.erase(iter);
		} else
			iter++;
	}
}

void BulletPool::createBullet(int x, int y, int vx, int vy){
//	std::cout << x << ", " << y << ", " << vx << ", " << vy << std::endl;

	if(this->freeList.empty() == true){
		Bullet* pb = new Bullet("bullet", Vector2f(x,y), Vector2f(vx, vy), 1);
		this->bullets.push_back(pb);
	}else{
		Bullet* pb = *this->freeList.begin();
		pb->init(Vector2f(x,y), Vector2f(vx, vy));

		this->freeList.pop_front();
//		std::cout << pb->getIsVisiable() << std::endl;
		this->bullets.push_back(pb);
	}
}
