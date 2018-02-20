#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>
#include "Actor.h"

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	virtual ~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	void newItem();
	void displayStatus();
	void addItem(Actor* x) { m_v.push_back(x); }
private:
	NachenBlaster* m_nach;
	std::vector<Actor*> m_v;
	int m_numVillains;
	int m_numVillainsDestroyed = 0;
};

#endif // STUDENTWORLD_H_
