#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir)
{
}

int StudentWorld::init()
{
	//create 30 stars
	for (int i = 0; i < 30; i++) {
		int x = randInt(0, VIEW_WIDTH - 1);
		int y = randInt(0, VIEW_HEIGHT - 1);
		int divSize = randInt(10, 100);
		double size = 5.0 / divSize;
		Star* p = new Star(x, y, size);
		m_v.push_back(p);
	}


    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	doSomthing();
	newItem();
	//decrement lives
    decLives();
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
}

void StudentWorld::doSomthing() {
	//do somthing for all the actors in the actor vector

	vector<Actor*>::iterator i = m_v.begin();
	while (i != m_v.end()) {
		(*i)->doSomething();
		if (!(*i)->getState()) {
			delete *i;
			i = m_v.erase(i);
		}
		else
			i++;
	}
}

void StudentWorld::newItem() {
	//introduce new stars
	int x = randInt(0, 14);
	if (x == 0) {
		int x = VIEW_WIDTH - 1;
		int y = randInt(0, VIEW_HEIGHT - 1);
		int divSize = randInt(10, 100);
		double size = 5.0 / divSize;
		Star* p = new Star(x, y, size);
		m_v.push_back(p);
	}
}
