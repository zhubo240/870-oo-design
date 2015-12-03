/*
 * EnemyPool.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: zhubo
 */

#include "EnemyPool.h"
#include "SmartEnemy.h"
#include "ioManager.h"

EnemyPool::EnemyPool(const Drawable* player) : isVisiable(true), enemies(),
freeList(),  player(player), viewport(Viewport::getInstance()), numEnemies(1)
{
}

void EnemyPool::reset(){
	for (std::list<SmartEnemy*>::iterator iter = this->enemies.begin();
				iter != this->enemies.end(); iter++) {
			delete *iter;
		}

		//freeList
		for (std::list<SmartEnemy*>::iterator iter = this->freeList.begin();
				iter != this->freeList.end(); iter++) {
			delete *iter;
		}

	this->enemies.resize(0);
	this->freeList.resize(0);
}



EnemyPool& EnemyPool::getInstance(const Drawable* player){
	//TODO:
	static EnemyPool pool(player);
	return pool;
}

void EnemyPool::draw() {
	for (std::list<SmartEnemy*>::iterator iter = this->enemies.begin();
			iter != this->enemies.end(); iter++) {
		(*iter)->draw();
	}
}

void EnemyPool::drawMsg(){
	if(this->isVisiable){
	//freeList
	int x  = 300, y = 70;
	std::stringstream s;
	s << this->freeList.size();
	IOManager::getInstance().printMessageAt("freelist size = " + s.str(), x, y);

	int i = 1;
	for (std::list<SmartEnemy*>::iterator iter = this->enemies.begin();
				iter != this->enemies.end(); iter++, i++) {
		std::stringstream ss;
		ss << "enemy" << i << " : " << "pos : " << (*iter)->getPosition()[0] << ", " << (*iter)->getPosition()[1]
												<< ", v :"<< (*iter)->getVelocity()[0] << ", " << (*iter)->getVelocity()[1];
		IOManager::getInstance().printMessageAt(ss.str(), x, i * 15 + y);
		}
	}

}

void EnemyPool::update(Uint32 ticks) {
	//enemies
	for (std::list<SmartEnemy*>::iterator iter = this->enemies.begin();
			iter != this->enemies.end(); iter++) {
		(*iter)->update(ticks);
	}

	//set IsVisiable if out of boundary
	for(std::list<SmartEnemy*>::iterator iter = this->enemies.begin();
			iter != this->enemies.end(); iter++){

		if(!(viewport.X() < (*iter)->X() &&
				(*iter)->X() < viewport.X() + viewport.getViewWidth() &&
				viewport.Y() < (*iter)->Y() &&
				(*iter)->Y() < viewport.Y() + viewport.getViewHeight())){
			(*iter)->setIsVisiable(false);
		}
	}

	//move enemy
	for (std::list<SmartEnemy*>::iterator iter = this->enemies.begin();
			iter != this->enemies.end();) {
		if ((*iter)->getIsVisiable() == false) {
			this->freeList.push_back(*iter);
			iter = this->enemies.erase(iter);
		} else
			iter++;
	}

	if(this->enemies.size() < 3){
		std::cout << "create enemy" << std::endl;
		this->createEnemy();

//		std::cout << "enemy num is " << this->enemies.size() << std::endl;
	}
}

void EnemyPool::createEnemy(){
	int x = viewport.X() + viewport.getViewWidth() - 50;
	int y = Gamedata::getInstance().getRandInRange(0, viewport.getViewHeight());

//	std::cout << x << ", " << y << ", " << vx << ", " << vy << std::endl;

	SmartEnemy* pb = new SmartEnemy("enemy", player);
	pb->setPosition(Vector2f(x,y));
	if(this->freeList.empty() == true){
		this->enemies.push_back(pb);
	}else{
		SmartEnemy* pb = *this->freeList.begin();
		pb->init(Vector2f(x,y));

		this->freeList.pop_front();
//		std::cout << pb->getIsVisiable() << std::endl;
		this->enemies.push_back(pb);
	}

}
